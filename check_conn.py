import xml.etree.ElementTree as ET

tree = ET.parse("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.hwh")
root = tree.getroot()

# Track which signals connect SA output ("m_axis" without "mm2s") to DMA S2MM input
for port in root.iter("PORT"):
    sig = port.get("SIGNAME", "")
    name = port.get("NAME", "")
    if "s2mm" in name.lower() and ("tdata" in name.lower() or "tvalid" in name.lower() or "tready" in name.lower() or "tlast" in name.lower()):
        print(f"Signal: {sig} ({name})")
        for conn in port.findall(".//CONNECTION"):
            inst = conn.get("INSTANCE", "")
            p = conn.get("PORT", "")
            print(f"  -> {inst}.{p}")
