📡 Lectura de memoria en tarjetas RFID/NFC (ESP32 + RC522)

Este proyecto implementa la lectura de memoria de tarjetas RFID/NFC utilizando un módulo RC522 conectado a una ESP32 mediante comunicación SPI.

🧠 Descripción

El programa detecta una tarjeta NFC y, en el caso de tarjetas MIFARE Classic, accede a su memoria interna organizada en sectores y bloques, permitiendo leer el contenido almacenado.

Cada bloque contiene 16 bytes, que se muestran en formato:

Hexadecimal
ASCII (texto legible cuando aplica)

🔧 Funcionalidad

Detección de tarjeta NFC
Identificación del tipo de tarjeta
Autenticación con clave por defecto (FFFFFFFFFFFF)
Lectura de bloques de memoria
Visualización de datos en HEX y ASCII
Detección de errores de autenticación

🧪 Ejemplo de salida

=== TARJETA DETECTADA ===
UID: 13 B9 D6 06
Tipo: MIFARE 1K

Bloque 04 | HEX: 48 6F 6C 61 20 45 53 50 33 32 20 52 43 35 32 32 | ASCII: Hola ESP32 RC522

🗂️ Estructura de la memoria (MIFARE Classic 1K)

16 sectores
4 bloques por sector
16 bytes por bloque

⚠️ El último bloque de cada sector es un bloque de seguridad (trailer) y contiene las claves de acceso.

⚠️ Limitaciones

Solo funciona con tarjetas MIFARE Classic
Requiere conocer la clave del sector (por defecto FFFFFFFFFFFF)
No es compatible con:
Tarjetas bancarias
Tarjetas tipo ISO/IEC 14443-4 (EMV)

En estos casos, la lectura fallará con errores de autenticación.

🔌 Hardware utilizado

ESP32
Módulo RFID RC522
Tarjetas MIFARE Classic 1K
⚙️ Conexión (SPI)
RC522	ESP32
SDA (SS)	GPIO 5
SCK	GPIO 18
MOSI	GPIO 23
MISO	GPIO 19
RST	GPIO 22
GND	GND
3.3V	3V3

🚀 Uso

Subir el código a la ESP32
Abrir monitor serie (115200 baudios)
Acercar una tarjeta MIFARE Classic
Observar la lectura de bloques

📌 Notas

No se deben modificar bloques de seguridad (3, 7, 11, etc.)
La lectura depende de que la clave sea correcta
El contenido puede no ser legible en ASCII si no es texto

🧠 Conclusión

Este proyecto demuestra cómo acceder a la memoria interna de tarjetas RFID tipo MIFARE Classic, permitiendo trabajar con datos en sistemas embebidos como control de acceso, almacenamiento de información o identificación.
