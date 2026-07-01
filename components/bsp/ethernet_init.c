#include <stdlib.h>

#include "ethernet_init.h"
#include "esp_check.h"
#include "esp_eth.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "ethernet_init";

/*
 * 初始化板载以太网底层驱动。
 * 当前硬件固定为 ESP32-P4 内部 EMAC + 板载 IP101 PHY，通过 RMII 连接。
 */
esp_err_t example_eth_init(esp_eth_handle_t *eth_handles_out[], uint8_t *eth_cnt_out)
{
    ESP_RETURN_ON_FALSE(eth_handles_out != NULL && eth_cnt_out != NULL, ESP_ERR_INVALID_ARG, TAG, "invalid arguments");

    esp_eth_handle_t *eth_handles = calloc(1, sizeof(esp_eth_handle_t));
    ESP_RETURN_ON_FALSE(eth_handles != NULL, ESP_ERR_NO_MEM, TAG, "no memory");

    esp_err_t ret = ESP_FAIL;
    esp_eth_mac_t *mac = NULL;
    esp_eth_phy_t *phy = NULL;
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
    eth_esp32_emac_config_t emac_config = ETH_ESP32_EMAC_DEFAULT_CONFIG();

    /*
     * 这些 GPIO 和 PHY 地址来自 BSP Kconfig。
     * ESP32-P4-Function-EV-Board 默认：MDC=31，MDIO=52，PHY_RST=51，PHY_ADDR=1。
     */
    phy_config.phy_addr = CONFIG_EXAMPLE_ETH_PHY_ADDR;
    phy_config.reset_gpio_num = CONFIG_EXAMPLE_ETH_PHY_RST_GPIO;
    emac_config.smi_gpio.mdc_num = CONFIG_EXAMPLE_ETH_MDC_GPIO;
    emac_config.smi_gpio.mdio_num = CONFIG_EXAMPLE_ETH_MDIO_GPIO;

    mac = esp_eth_mac_new_esp32(&emac_config, &mac_config);
    ESP_GOTO_ON_FALSE(mac != NULL, ESP_FAIL, err, TAG, "create Ethernet MAC failed");

    phy = esp_eth_phy_new_ip101(&phy_config);
    ESP_GOTO_ON_FALSE(phy != NULL, ESP_FAIL, err, TAG, "create IP101 PHY failed");

    /* MAC 和 PHY 都创建成功后，安装 ESP-IDF Ethernet driver，返回给上层绑定 esp-netif。 */
    esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, phy);
    ESP_GOTO_ON_ERROR(esp_eth_driver_install(&config, &eth_handles[0]), err, TAG, "install Ethernet driver failed");

    *eth_handles_out = eth_handles;
    *eth_cnt_out = 1;
    return ESP_OK;

err:
    if (mac != NULL) {
        mac->del(mac);
    }
    if (phy != NULL) {
        phy->del(phy);
    }
    free(eth_handles);
    return ret;
}
