# ไฟล์: components/mcp2515_lib/CMakeLists.txt

idf_component_register(
    SRCS "mcp2515.cpp"
    INCLUDE_DIRS "include"           # ย้ายมาไว้ตรงนี้ เพื่อให้ main และตัวมันเองหาเจอ
    PRIV_REQUIRES     # ต้องใส่เพื่อให้รู้จัก SPI และ uint8_t ของระบบ
)