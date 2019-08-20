G91; Relative Position
G1 Z5; Move away from the part
M104 S0; Turn off hotent
M140 S0; Turn off bed
G1 X180 Y300; Move hotend to center back
M84; Disable steppers

M300 S440 P200; Play tune
M300 S660 P250
M300 S880 P300