"""Acceptance tests for the BLE link, run from a PC against a flashed ESP32-S3.

These are hardware-in-the-loop tests. They need the board powered, in range,
and not connected to anything else -- a BLE peripheral accepts one connection
at a time, so disconnect nRF Connect before running them.

They test the link contract the slow brain will code against: that the board is
findable, that its GATT table has the agreed UUIDs and permissions, and that a
command written down produces a status coming back up. They do not test any
firmware logic, timing, or servos.
"""

import asyncio

import pytest
from bleak import BleakClient, BleakScanner

DEVICE_NAME = "FIZMO_FB"
SERVICE_UUID = "8d707e01-adb4-4868-b3c3-f9f65611f14c"
COMMAND_UUID = "8d707e02-adb4-4868-b3c3-f9f65611f14c"
STATUS_UUID = "8d707e03-adb4-4868-b3c3-f9f65611f14c"

SCAN_TIMEOUT_S = 10.0
NOTIFY_TIMEOUT_S = 5.0

# The default ATT MTU is 23 bytes, of which 3 are header. A write longer than
# the remaining 20 is the interesting case.
LONG_COMMAND = b"1,WALK,0.5,0.0,0.0,0.0,0.0,0.0,0.0"


async def _find_device():
    device = await BleakScanner.find_device_by_name(DEVICE_NAME, timeout=SCAN_TIMEOUT_S)
    if device is None:
        pytest.fail(
            f"{DEVICE_NAME} not found in {SCAN_TIMEOUT_S}s. Board powered? "
            "Already connected to a phone?"
        )
    return device


@pytest.fixture(scope="session")
def device():
    return asyncio.run(_find_device())


def test_advertises_and_connects(device):
    async def scenario():
        async with BleakClient(device) as client:
            assert client.is_connected

    asyncio.run(scenario())


def test_gatt_table_has_agreed_shape(device):
    async def scenario():
        async with BleakClient(device) as client:
            service = client.services.get_service(SERVICE_UUID)
            assert service is not None, "fizmo_commands service missing"

            uuids = {c.uuid.lower() for c in service.characteristics}
            assert uuids == {COMMAND_UUID, STATUS_UUID}

    asyncio.run(scenario())


def test_command_is_writable_and_status_is_readable(device):
    async def scenario():
        async with BleakClient(device) as client:
            command = client.services.get_characteristic(COMMAND_UUID)
            status = client.services.get_characteristic(STATUS_UUID)

            assert "write" in command.properties
            assert "read" in status.properties
            assert "notify" in status.properties

    asyncio.run(scenario())


def test_status_reads_back_non_empty(device):
    async def scenario():
        async with BleakClient(device) as client:
            value = await client.read_gatt_char(STATUS_UUID)
            assert len(value) > 0

    asyncio.run(scenario())


def test_command_accepts_a_write(device):
    async def scenario():
        async with BleakClient(device) as client:
            await client.write_gatt_char(COMMAND_UUID, b"1,CHECK_STATE", response=True)

    asyncio.run(scenario())


def test_command_accepts_a_write_longer_than_the_default_mtu(device):
    async def scenario():
        async with BleakClient(device) as client:
            assert len(LONG_COMMAND) > 20
            await client.write_gatt_char(COMMAND_UUID, LONG_COMMAND, response=True)

    asyncio.run(scenario())


def test_write_to_command_produces_a_notification_on_status(device):
    async def scenario():
        received = asyncio.Event()
        notifications = []

        def on_status(_handle, data: bytearray):
            notifications.append(bytes(data))
            received.set()

        async with BleakClient(device) as client:
            await client.start_notify(STATUS_UUID, on_status)
            await client.write_gatt_char(COMMAND_UUID, b"7,CHECK_STATE", response=True)
            await asyncio.wait_for(received.wait(), timeout=NOTIFY_TIMEOUT_S)
            await client.stop_notify(STATUS_UUID)

        assert notifications, "no notification arrived on STATUS"

    asyncio.run(scenario())


def test_advertises_again_after_a_disconnect(device):
    async def scenario():
        async with BleakClient(device) as client:
            assert client.is_connected

        found = await BleakScanner.find_device_by_name(
            DEVICE_NAME, timeout=SCAN_TIMEOUT_S
        )
        assert found is not None, "stopped advertising after the client left"

    asyncio.run(scenario())


def test_survives_three_connect_cycles(device):
    async def scenario():
        for attempt in range(3):
            async with BleakClient(device) as client:
                assert client.is_connected, f"connection {attempt + 1} failed"
            await asyncio.sleep(0.5)

    asyncio.run(scenario())


def test_report_negotiated_mtu(device):
    """Not a pass/fail check so much as a measurement we have never taken.

    The connection interval, the other unmeasured number, is not exposed by
    bleak; the board prints it from onConnect.
    """

    async def scenario():
        async with BleakClient(device) as client:
            mtu = client.mtu_size
            print(f"\nnegotiated ATT MTU: {mtu} bytes ({mtu - 3} usable payload)")
            assert mtu >= 23

    asyncio.run(scenario())
