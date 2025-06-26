# optee_project

개인 optee 프로그램을 저장하는 저장소

### 파일/폴더 설명


<pre><code>
📦optee_my_gpio
 ┣ 📂.git
 ┣ 📂.vscode
 ┃ ┣ 📜c_cpp_properties.json
 ┃ ┣ 📜launch.json
 ┃ ┣ 📜settings.json
 ┃ ┗ 📜tasks.json
 ┣ 📂host
 ┃ ┣ 📂build
 ┃ ┣ 📜Makefile               //
 ┃ ┗ 📜main.cpp               //
 ┣ 📂lib                       // rpi3에 설치된 optee에서 사용할 수 있는 자체 제작 라이브러리
 ┃ ┣ 📂ksj_gpio               // gpio 제어 라이브러리
 ┃ ┃ ┣ 📂build
 ┃ ┃ ┣ 📂include
 ┃ ┃ ┃ ┗ 📜ksj_gpio.h       // 
 ┃ ┃ ┣ 📂obj
 ┃ ┃ ┣ 📂src
 ┃ ┃ ┃ ┣ 📜ksj_gpio.c       // 
 ┃ ┃ ┃ ┗ 📜test.c           // 
 ┃ ┃ ┗ 📜Makefile
 ┃ ┗ 📂ksj_onnxruntime        // onnx runtime 라이브러리
 ┃ ┃ ┣ 📂build
 ┃ ┃ ┣ 📂include
 ┃ ┃ ┃ ┣ 📜Layer.h          // layer 정의 헤더
 ┃ ┃ ┃ ┣ 📜LayerType.h      // layer 타입 정의 헤더
 ┃ ┃ ┃ ┣ 📜Tensor.h         // 연산에 필요한 tensor class
 ┃ ┃ ┃ ┣ 📜activation.h     // activation layer 선언
 ┃ ┃ ┃ ┣ 📜conv.h           // cnn layer 선언
 ┃ ┃ ┃ ┣ 📜defines.h        // 여러 config 정의
 ┃ ┃ ┃ ┣ 📜linear.h         // linear layer 선언
 ┃ ┃ ┃ ┣ 📜model.h          // 
 ┃ ┃ ┃ ┣ 📜parson.h         // 
 ┃ ┃ ┃ ┣ 📜pool.h           // 
 ┃ ┃ ┃ ┗ 📜run.h            // 
 ┃ ┃ ┣ 📂obj
 ┃ ┃ ┣ 📂src
 ┃ ┃ ┃ ┣ 📜Tensor.c         //
 ┃ ┃ ┃ ┣ 📜activation.c     //
 ┃ ┃ ┃ ┣ 📜conv.c           //
 ┃ ┃ ┃ ┣ 📜linear.c         //
 ┃ ┃ ┃ ┣ 📜parson.c         //
 ┃ ┃ ┃ ┣ 📜pool.c           //
 ┃ ┃ ┃ ┣ 📜run.c            //
 ┃ ┃ ┃ ┗ 📜test.c           //
 ┃ ┃ ┗ 📜Makefile            //
 ┣ 📂ta
 ┃ ┣ 📂build
 ┃ ┣ 📂include
 ┃ ┃ ┗ 📜my_gpio_ta.h              //
 ┃ ┣ 📜Makefile                     //
 ┃ ┣ 📜my_gpio_ta.c                 // 
 ┃ ┣ 📜sub.mk                       // 
 ┃ ┗ 📜user_ta_header_defines.h     //
 ┣ 📂testcases                       //
 ┣ 📂tools                           // 
 ┃ ┣ 📜load_onnx.py                 // 
 ┃ ┗ 📜test_case_generator.py       // 
 ┣ 📜.gitignore                      //
 ┣ 📜Android.mk
 ┣ 📜CMakeLists.txt                  // 
 ┣ 📜Makefile                        // 
 ┣ 📜README.md                       // 
 ┗ 📜compile.txt                     // 

</code></pre>