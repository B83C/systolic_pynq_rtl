#!/bin/bash
# Run on PYNQ: source env and run yunet_pynq.py
source /etc/profile.d/pynq_venv.sh
source /etc/profile.d/xrt_setup.sh
cd /home/xilinx/jupyter_notebooks/test_systolic_vivado_manual
python3 yunet_pynq.py "$@"
