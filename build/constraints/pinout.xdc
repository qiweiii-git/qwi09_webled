#system.xdc

#clock
set_property IOSTANDARD LVCMOS33 [get_ports CLK_50M]
set_property PACKAGE_PIN U18 [get_ports CLK_50M]
create_clock -period 20.000 -waveform {0.000 10.000} [get_ports CLK_50M]

#led indicator
set_property IOSTANDARD LVCMOS33 [get_ports LED]
set_property PACKAGE_PIN M15 [get_ports LED]
