| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-H4 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

## 🚀 Código de Transmissão (TX) Principal: `main.c`

Este é o ponto de entrada (`app_main`) do firmware do nó transmissor, desenvolvido nativamente sobre o framework **ESP-IDF v5.x**. O código foi projetado de forma portável para atender tanto a placa **Heltec WiFi LoRa 32 V2** quanto a **LilyGo WiFi & LoRa v1.0**.

### 🛠️ Principais Funcionalidades do Firmware

1. **Ativação do Rádio por Hardware (GPIO 21):**
   * Inicializa o pino GPIO 21 como saída digital e o define em nível lógico baixo (`LOW`). Essa etapa física é **obrigatória** nessas placas para alimentar o barramento de energia do chip de rádio Semtech (SX1276).

2. **Parâmetros de Modulação LoRa Configurados:**
   * **Frequência (915 MHz):** Canal padrão regulamentado e liberado para uso no Brasil (Anatel) e Américas.
   * **Potência de Saída (17 dBm):** Configurada no limite máximo estável para garantir o maior alcance de sinal possível em testes de campo.
   * **Fator de Espalhamento (SF7):** Varredura de rádio rápida para otimizar o tempo no ar (*Time-on-Air*) e reduzir drasticamente o consumo de bateria do circuito.
   * **Largura de Banda (125 kHz):** Configuração clássica que equilibra perfeitamente a taxa de dados com a imunidade a ruídos externos.

3. **Integridade de Rede:**
   * **Ativação de CRC por hardware:** Garante que o rádio receptor descarte pacotes que sofram qualquer tipo de interferência magnética ou colisão no ar.
   * **Explicit Header Mode:** Envia as informações do tamanho do payload e configurações de CRC no início do pacote, facilitando a sincronização rápida do receptor.

4. **Rotina de Envio:**
   * O rádio entra em um loop infinito enviando a string `"Olá Mundo! [contador]"` a cada 5 segundos, acompanhado de logs detalhados via `ESP_LOGI` para facilitar o monitoramento em tempo real pelo terminal serial.

### ⚙️ Portabilidade: Como alternar entre Heltec V2 e LilyGo v1.0

Embora ambas as placas compartilhem o mesmo transceptor LoRa, o pino de **Reset (RST)** do rádio está conectado a GPIOs diferentes em cada circuito. 

Para alternar o firmware de uma placa para a outra, basta abrir o arquivo de configuração do driver (geralmente o `lora.c` ou o cabeçalho de pinagem) e **comentar/descomentar** as linhas correspondentes ao pino de Reset:

```c
// Hardware Pin Mappings for ESP-IDF v5.x
#define CONFIG_MOSI_GPIO 27
#define CONFIG_MISO_GPIO 19
#define CONFIG_SCK_GPIO  5
#define CONFIG_CS_GPIO   18

/* --- CONFIGURAÇÃO DO PINO DE RESET (RST) --- */
#define CONFIG_RST_GPIO  14  /* ◄ Descomente para usar a placa Heltec V2 */
// #define CONFIG_RST_GPIO  23 /* ◄ Descomente para usar a placa LilyGo v1.0 */
