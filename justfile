
test tb="sa_tb": 
  verilator --cc {{tb}}.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 8 --hierarchical  --timing --binary --Mdir {{tb}}_obj/
  cd ./{{tb}}_obj/ && ./V{{tb}}

wave tb="sa_tb":
  surfer ./{{tb}}_obj/waveform.fst --state {{tb}}_state

upload num="1":
  ls -la /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh
  ls -la /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit
  scp /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.hwh
  scp /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit
  scp systolic.py pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.py
  
