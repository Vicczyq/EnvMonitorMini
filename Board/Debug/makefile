#create:2018-10-12
#auther:zsl,wb

#�����Ķ����ļ���֪ʶ
#(1)�ַ�'\'��ʾ����
#(2)����Ŀ��ĵ�һ��ָ��ǰ������һ��tab��������4���ո����
#(3)$@  ��ʾĿ���ļ�
#   $^  ��ʾ���е������ļ�
#   $<  ��ʾ��һ�������ļ�
#   $?  ��ʾ��Ŀ�껹Ҫ�µ������ļ��б�

#���屾�ļ���ʹ�õ��ı���������ʵ����Ҫ�޸ġ�
#����̨�������·��
printf    := echo

#.C�ļ��ļ���
SRC_C := ./srcc
#.S�ļ��ļ���
SRC_S := ./srcs
#.o�ļ����Ŀ¼
D_OBJ := ./obj
#.d�ļ����Ŀ¼
D_MK  := ./dmk
#.C�ļ��б�
SRC_CList := $(wildcard ./srcc/*.c)
#.CPP�ļ��б�
#��20200305 1/8�� ������&���Ʋ� ����.cpp���ļ��б�������.c�ļ��б�����
SRC_CPPList := $(wildcard ./srcc/*.cpp)
#.s�ļ��б�
#��20200305 2/8�� ������&���Ʋ� ����.S�ļ��б�������ԭ��.s��.S��������
SRC_sList := $(wildcard ./srcs/*.s)
#.S�ļ��б�
SRC_SList := $(wildcard ./srcs/*.S)

#�����ļ�(.ld)�Ĵ洢·��
linkFile := "C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\03_MCU\Linker_file\STM32L431RCTX_FLASH.ld"
#�������ɵ��ļ���
finalGoal := AHL-STM32L431-Test-20230811
#����ͷ�ļ����ڵ�·��
include := -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\.settings" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\01_Doc" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\02_CPU" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\03_MCU" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\04_GEC" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\05_UserBoard" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\06_SoftComponent" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\07_AppPrg" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\Debug" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\03_MCU\Linker_file" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\03_MCU\MCU_drivers" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\03_MCU\startup" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\Debug\obj" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\Debug\srcc" -I"C:\Users\Vicczyq\Desktop\AHL-STM32L431-Test-20230811\Debug\srcs"
#�����Ż����𡣹���O0��O1��O2��Os��Og��Ĭ��ʹ��Og
opt := Og
#.o�ļ��б�
#��20200305 3/8�� ������&���Ʋ� ��ԭ����OBJ_C��ΪOBJ-CS������.S��.o�����ļ�

#��.c�ļ��滻Ϊ.o�ļ�
OBJ_CS := $(addprefix $(D_OBJ)/,$(patsubst %.c,%.o,$(notdir $(SRC_CList))))
#��.cpp�ļ��滻Ϊ.o�ļ�
OBJ_CS += $(addprefix $(D_OBJ)/,$(patsubst %.cpp,%.o,$(notdir $(SRC_CPPList))))
#��.s�ļ��滻Ϊ.o�ļ�  
OBJ_CS += $(addprefix $(D_OBJ)/,$(patsubst %.s,%.o,$(notdir $(SRC_sList))))
#��.S�ļ��滻Ϊ.o�ļ�  
OBJ_CS += $(addprefix $(D_OBJ)/,$(patsubst %.S,%.o,$(notdir $(SRC_SList))))

#.d�ļ��б�
#��20200305 4/8�� ������&���Ʋ� ����.S��.d�����ļ�

#��.c�ļ��滻Ϊ.d�ļ�
SRC_MK := $(addprefix $(D_OBJ)/, $(patsubst %.c,%.d,$(notdir $(SRC_CList))))
#��.cpp�ļ��滻Ϊ.d�ļ�
SRC_MK += $(addprefix $(D_OBJ)/, $(patsubst %.cpp,%.d,$(notdir $(SRC_CPPList))))
#��.s�ļ��滻Ϊ.d�ļ�
SRC_MK += $(addprefix $(D_OBJ)/, $(patsubst %.s,%.d,$(notdir $(SRC_sList))))
#��.S�ļ��滻Ϊ.d�ļ�  
SRC_MK += $(addprefix $(D_OBJ)/, $(patsubst %.S,%.d,$(notdir $(SRC_SList))))


#���ļ���./Source�е�.c�ļ�����Ϊ.o�ļ�
#��20200305 5/8�� ������&���Ʋ� �����ȼ�-O0�ĳ�-$(opt),�������о��޸�
$(D_OBJ)/%.o:$(SRC_C)/%.c
	arm-none-eabi-gcc "$^" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG \
-DSTM32L431xx -c $(include) -$(opt) -ffunction-sections -fdata-sections -Wall -fstack-usage \
-MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" 

#���ļ���./Source�е�.cpp�ļ�����Ϊ.o�ļ�
#��20200305 6/8�� ������&���Ʋ� ����.cpp�ı����.o�ļ��Ĺ����ʽ
$(D_OBJ)/%.o:$(SRC_C)/%.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -c $(include) -$(opt) \
-ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics \
-fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)"  -MT"$@" \
--specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

#���ļ���./Source�е�.s�ļ�����Ϊ.o�ļ� 
$(D_OBJ)/%.o:$(SRC_S)/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c $(include) -x assembler-with-cpp \
--specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

#���ļ���./Source�е�.S�ļ�����Ϊ.o�ļ� 
#��20200305 7/8�� ������&���Ʋ� ����.S�ı����.o�ļ��Ĺ����ʽ
$(D_OBJ)/%.o:$(SRC_S)/%.S
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c $(include) -x assembler-with-cpp \
--specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"




# �����ļ��е�Ŀ�ꡣĿ��Ϊϣ�����ɵ��ļ������С�all���͡�clean��ΪαĿ��
all:default.size.stdout $(finalGoal).lst $(finalGoal).hex 

#��20200305 8/8�� ������&���Ʋ� ��ԭ����gcc��Ϊg++,����OBJ_C��ΪOBJ_CS

$(finalGoal).elf: $(OBJ_CS)
	arm-none-eabi-g++ -o "$(finalGoal).elf" $(OBJ_CS) -mcpu=cortex-m4 -T $(linkFile) \
--specs=nosys.specs -Wl,-Map="$(finalGoal).map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 \
-mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@$(printf) 'Finished building target: $@'
	@$(printf) ' '

$(finalGoal).hex: $(finalGoal).elf $(finalGoal).lst
	arm-none-eabi-objcopy -O ihex "$<"  "$@"
	@$(printf) 'Finished building: $@'
	@$(printf) ' '

# ��20200604 5/5�� �鿴оƬ�ڴ�ʹ�����
default.size.stdout: $(finalGoal).elf
	arm-none-eabi-size  $(finalGoal).elf
	@echo 'Finished building: $@'
	@echo ' '
	
$(finalGoal).lst: $(finalGoal).elf
	arm-none-eabi-objdump -h -S "$(finalGoal).elf" > "$(finalGoal).lst"
	@$(printf) 'Finished building: $@'
	@$(printf) ' '

#�������������ռ��αĿ��
#��ɾ��Ŀ���ļ��������ļ���δ�����䶯�򲻱��룬��������߱����ٶ�
fastClean:
	rm -rf "$(finalGoal).elf" "$(finalGoal).hex" "$(finalGoal).map"
	-@$(printf) 'make: fast clean has be done'
	-@$(printf) ' '
#ɾ�������м��ļ���Ŀ���ļ���ȫ�����±���	
clean:
	rm -rf  "$(finalGoal).elf" "$(finalGoal).hex" "$(finalGoal).map" "$(finalGoal).lst" "$(finalGoal).hex" $(OBJ_C) $(SRC_MK)
	-@$(printf) 'make: clean has be done'
	-@$(printf) ' '
test:
	arm-none-eabi-size  $(finalGoal).elf
	@$(printf) 'Finished building: $@'
	@$(printf) ' '
	-@$(printf)  $(OBJ_C)
	
#ָ����ЩĿ��ΪαĿ��
.PHONY: all clean fastClean test

