set export := true

OBJCACHE := "ccache"
# CXX := "ccache g++"

test tb="sa_tb": 
  verilator --cc {{tb}}.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 16 --threads 4 --hierarchical --timing --binary --Mdir {{tb}}_obj/
  cd ./{{tb}}_obj/ && ./V{{tb}}

wave tb="sa_tb":
  surfer ./{{tb}}_obj/waveform.fst --state-file {{tb}}_state

test-dw:
  verilator --cc dw3x3_axis_tb.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold -CFLAGS -std=c++20 --verilate-jobs 8 --timing --binary --Mdir dw3x3_axis_tb_obj/ -Wno-UNOPTFLAT -Wno-WIDTHTRUNC -Wno-INITIALDLY
  cd ./dw3x3_axis_tb_obj/ && ./Vdw3x3_axis_tb

wave-dw:
  surfer ./dw3x3_axis_tb_obj/waveform.fst --state-file dw3x3_axis_tb_state

upload num="1":
  ls -la /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh
  ls -la /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit
  scp /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.hwh
  scp /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit
  scp systolic.py pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.py
  
