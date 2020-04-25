#add constraints
add_files -fileset constrs_1 -norecurse ./constraints/pinout.xdc

#add source
add_files ../source/qwi09_webled.sv
add_files ../source/qwiregctrl.v
add_files ../source/define

#global define
#set_property is_global_include true [get_files define.vh]
