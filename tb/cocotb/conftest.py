"""Pytest config: skip tests that need RTL simulation."""
import pytest

collect_ignore = ["test_sa.py", "cocotb_test.py"]

