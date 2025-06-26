cmake_minimum_required(VERSION 3.16)

project(ksj_onnxruntime_cpp LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 기본 빌드 타입
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# 소스 및 헤더 파일 분리 정의
set(SRC_FILES
    src/main.cpp
    src/inference.cpp
)

# include 폴더 설정 (헤더파일 경로)
include_directories(${PROJECT_SOURCE_DIR}/include)


# 실행 파일 추가
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 라이브러리 링크
