import xml.etree.ElementTree as ET

tree = ET.parse("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.hwh")
root = tree.getroot()

for port in root.iter("PORT"):
    sig = port.get("SIGNAME", "")
    name = port.get("NAME", "")
    if "mm2s" in name.lower() and ("tdata" in name.lower()):
        print(f"MM2S signal: {sig} ({name})")
        for conn in port.findall(".//CONNECTION"):
            inst = conn.get("INSTANCE", "")
            p = conn.get("PORT", "")
            print(f"  -> {inst}.{p}")

for port in root.iter("PORT"):
    name = port.get("NAME", "")
    if name.lower().startswith("s_axis_a_"):
        print(f"\nSA A input port: {port.get('SIGNAME','')} ({name})")
        for conn in port.findall(".//CONNECTION"):
            print(f"  -> {conn.get('INSTANCE','')}.{conn.get('PORT','')}")

for port in root.iter("PORT"):
    name = port.get("NAME", "")
    if name.lower().startswith("m_axis_mm2s_"):
        print(f"\nMM2S output port: {port.get('SIGNAME','')} ({name})")
        for conn in port.findall(".//CONNECTION"):
            print(f"  -> {conn.get('INSTANCE','')}.{conn.get('PORT','')}")
