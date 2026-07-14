#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdio.h>
#include "lora.h"

static const char *TAG = "LoRa_TX";

void app_main(void)
{
    ESP_LOGI(TAG, "===== Nó LoRa TX - Heltec V2 / LilyGo =====");

    // Pull GPIO 21 LOW to turn on the LoRa chip power rail
    gpio_reset_pin(21);
    gpio_set_direction(21, GPIO_MODE_OUTPUT);
    gpio_set_level(21, 0); 
    vTaskDelay(pdMS_TO_TICKS(50)); // Wait for the chip to wake up
    

    // Inicialización SPI y LoRa
    if (!lora_init()) {
        ESP_LOGE(TAG, "❌ Não foi possível conectar ao módulo LoRa");
        while (1) vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Configuração do LoRa
    lora_set_frequency(915E6);       // Frequência em 915 MHz (padrão regulamentado para o Brasil e Américas)
    lora_set_tx_power(17);           // Potência de transmissão em 17 dBm (máximo do chip para maior alcance)
    lora_set_spreading_factor(7);    // Fator de Espalhamento SF7 (transmissão rápida, menor tempo no ar e menor consumo)
    lora_set_bandwidth(125E3);       // Largura de banda em 125 kHz (equilíbrio ideal entre taxa de dados e imunidade a ruídos)
    lora_enable_crc();               // Ativa verificação de erros por hardware (descarta pacotes corrompidos no ar)
    lora_explicit_header_mode();     // Inclui metadados (tamanho do pacote e CRC) no início da transmissão para o receptor


    ESP_LOGI(TAG, "✅ LoRa configurado. Iniciando transmissão...");

    uint32_t counter = 0;
    char msg[64];

    while (1) {
        snprintf(msg, sizeof(msg), "Olá Mundo! [%lu]", counter++);
        ESP_LOGI(TAG, "Enviando pacote Lora...");
        lora_send_packet((uint8_t*)msg, strlen(msg));
        ESP_LOGI(TAG, "📤 Enviado: \"%s\"", msg);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Envio a cada 5 segundos
    }

}
