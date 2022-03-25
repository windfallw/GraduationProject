import platform
Import("env")

print("Replace MKSPIFFSTOOL with mklittlefs")

if platform.system()=="Windows":
    env.Replace(MKSPIFFSTOOL="mklittlefs.exe")
elif platform.system()=="Darwin":
    env.Replace(MKSPIFFSTOOL="./mklittlefs")
