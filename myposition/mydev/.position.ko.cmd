cmd_/home/pi/myposition/mydev/position.ko := ld -r  -EL -T ./scripts/module-common.lds -T ./arch/arm/kernel/module.lds  --build-id  -o /home/pi/myposition/mydev/position.ko /home/pi/myposition/mydev/position.o /home/pi/myposition/mydev/position.mod.o ;  true
