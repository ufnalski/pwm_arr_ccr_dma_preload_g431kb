# [CMSIS mini-tutorials] Symmetrical PWM DMA ARR CCR preload from the very first period [STM32G431KB]

What if the HAL library does not offer a specific sequence of actions needed to interact with a peripheral on your own terms? No worries! We have CMSIS (Common Microcontroller Software Interface Standard). You can interact with your STM32 microcontroller at the register level using user-friendly predefined names/identifiers. Now everything is possible! What if ```HAL_TIM_PWM_Start_DMA()``` is not an option for you because it starts the DMA channel and the TIM peripheral consecutively without letting you to do something in between? For example, imagine you need ARR nad CCR shadow registers to have specific values before the TIM starts. No problem! Remember, everything that is not prohibited by the silicone at hand is now possible :sunglasses:

![PWM DMA ARR CCR preload in action](/Assets/Images/arr_ccr_dma_preload_in_action.jpg)
![PWM DMA ARR CCR preload DSView](/Assets/Images/pwm_arr_ccr_dma_dsview.JPG)

# Missing files?
Don't worry. Just open the .ioc file in STM32CubeMX and hit the GENERATE CODE blue bar. Then open the project in STM32CubeIDE for Visual Studio Code and build it.

# Readings
* [Common Microcontroller Software Interface Standard (CMSIS)](https://www.arm.com/technologies/cmsis) (Arm)
* [CMSIS Version 6.3.x](https://arm-software.github.io/CMSIS_6/latest/General/index.html) (Arm)
* [CMSIS_6](https://github.com/ARM-software/CMSIS_6) (Arm) 
* [STM32 Na Rejestrach](https://gitlab.com/stm32-na-rejestrach) (Mateusz Salamon)
* [RM0440 (Reference manual): STM32G4 series advanced Arm-based 32-bit MCUs](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) (STMicroelectronics)
* [STM32 timer detailed explanation - shadow register, pre-loaded register](https://en.eeworld.com.cn/news/mcu/eic283069.html) (EEWORLD)
* [Wrong interpretation of shadow register?](https://community.st.com/stm32-mcus-products-25/wrong-interpretation-of-shadow-register-94988) (ST Community)
* [Preventing Race Conditions in Embedded Systems](https://www.linkedin.com/posts/robota-corporation-pvt-ltd_embeddedsystems-stm32-rtos-activity-7495346506548326400-a8xt) (Robota Corporation Pvt Ltd)
* [Race condition](https://en.wikipedia.org/wiki/Race_condition) (Wikipedia)

# Call to action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2026_festiwal_nauki/Festiwal_Nauki_2026.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [bitluni's lab](https://www.youtube.com/@bitluni), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Max Imagination](https://www.youtube.com/@MaxImagination), [Nikodem Bartnik](https://www.youtube.com/@nikodembartnik), [Stuff Made Here](https://www.youtube.com/@StuffMadeHere), [Mario's Ideas](https://www.youtube.com/@marios_ideas), [Aaed Musa](https://www.aaedmusa.com/), [Haase Industries](https://www.youtube.com/@h1tec), [Marcin Plaza](https://www.youtube.com/MarcinPlaza), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them! Promote [README-driven learning](http://ufnalski.edu.pl/proceedings/sene2025/Ufnalski_PE_formatted_SENE_2025.pdf).

> [!WARNING]
> CMSIS let's you play with an STM32 at the register level - do try this at home :grey_exclamation:

230 challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!

0xBU
