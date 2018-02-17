if {$argc != 1} {exit 1}

set fname [lindex $argv 0]
puts $fname
open_hw
connect_hw_server -url localhost:3121
current_hw_target [get_hw_targets */xilinx_tcf/Digilent/12345]
open_hw_target

current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false [lindex [get_hw_devices] 0]
set_property PROGRAM.FILE ${fname} [lindex [get_hw_devices] 0]
puts [get_property PROGRAM.FILE [lindex [get_hw_devices] 0]]
program_hw_devices [lindex [get_hw_devices] 0]
refresh_hw_device [lindex [get_hw_devices] 0]
