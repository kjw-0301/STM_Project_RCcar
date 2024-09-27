# STM_Project_RCcar
### 모든 프로젝트의 전반적인 내용은 다음 링크를 통해서 확인하실 수 있습니다.
- [🚗 Visit RC_Car Project Docs](https://www.canva.com/design/DAGPbfohr20/pTF_LQDb0HObs8oqjEBFqg/view?utm_content=DAGPbfohr20&utm_campaign=designshare&utm_medium=link&utm_source=editor)   
## RC_Car_Phone_BT(블루투스 4.0)
- 휴대폰의 "Dabble" 앱의 조이스틱 모듈을 통해서 RC카의 제어를 할 수 있습니다.
<br>    
| 파일명 | 설명|
| --- | --- |
| **main.c** | **DMA**방식을 사용해 **UART**통신을 하여 앱의 조이스틱에서 입력한 값으로 모터의 동작을 제어합니다.| 
| **motor.h** | **motor.c**에서 사용할 함수와 변수, 전처리기를 정의합니다.| 
| **motor.c** | **motor.h**에서 정의된 함수 및 변수들을 사용해 실제 동작할 함수의 내용을 작성한 파일입니다.| 

<br>

## RC_Car_Project_ADC_JoyStick(HC-06(Slave)) & RC_Car_project_joystick_BT(HC-05(Master))
####  **RC_Car_Project_ADC_JoyStick**
- **HC-06(Slave)** 모듈을 연결하여 조이스틱 기능을 수행합니다. 
- 조이스틱의 값을 **ADC**변환하여 **Uart통신**으로 변환된 값을 **HC-05(Master)** 모듈로 전송합니다.
#### **RC_Car_project_joystick_BT**
-  **HC-05(Master)** 와 연결하여 **Uart통신**으로 **HC-06(Slave)** 모듈이 전송한 값을 전송받습니다. 
- 전송받은 데이터는 **rxData**에 저장되며 **저장된 값에 따라서 모터의 동작을 제어**하여 **전진,후진,좌회전,우회전,속도제어뤼튼**의 동작을 수행합니다.

>**HC-06(Slave)** 에서 보낸 **txData**의 값과 **HC-05(Master)** 에서 받아 **rxData**에 저장한 값이 다른 이유
>- 문제의 요지는 다음과 같습니다. 우리는 RC카의 **STM32F411RETX** 보드에서 **USART6**의 **BaudRate**를 **9600**으로 설정하여 **HC-06(Slave)** 와 동일하게 맞췄습니다. 그러나 현재 두 개의 블루투스 모듈 간 통신이 이루어지고 있는 상황에서, **HC-05 모듈**은 **무선 통신 시 BaudRate가 38400으로 고정**되어 있습니다. 이로 인해 통신 속도가 일치하지 않아 **rxData**에 저장된 문자가 깨지는 문제가 발생하고 있습니다. 이 문제를 해결하기 위해서는 **HC-05**의 **BaudRate**를 조정하거나, 다른 모듈을 사용하는 방법을 고려해야 합니다.
