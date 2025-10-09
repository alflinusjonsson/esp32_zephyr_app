#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>

BUILD_ASSERT(sizeof(CONFIG_WIFI_SAMPLE_SSID) > 1,
	"CONFIG_WIFI_SAMPLE_SSID is empty. Please provide it to west using -DCONFIG_WIFI_SAMPLE_SSID=<your-ssid> or set it in prj.conf.");
BUILD_ASSERT(sizeof(CONFIG_WIFI_SAMPLE_PSK) > 1,
	"CONFIG_WIFI_SAMPLE_PSK is empty. Please provide it to west using -DCONFIG_WIFI_SAMPLE_PSK=<your-psk> or set it in prj.conf.");

LOG_MODULE_REGISTER(MAIN);

#define NET_EVENT_WIFI_MASK (NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT)

static struct net_if *sta_iface;
static struct wifi_connect_req_params sta_config;
static struct net_mgmt_event_callback cb;

static void on_wifi_mgmt_event(struct net_mgmt_event_callback *cb, uint64_t mgmt_event,
							   struct net_if *iface)
{
	LOG_INF("Entered %s", __func__);

	switch (mgmt_event)
	{
	case NET_EVENT_WIFI_CONNECT_RESULT: {
		LOG_INF("Connected to %s", CONFIG_WIFI_SAMPLE_SSID);
		break;
	}
	case NET_EVENT_WIFI_DISCONNECT_RESULT: {
		LOG_INF("Disconnected from %s", CONFIG_WIFI_SAMPLE_SSID);
		break;
	}
	default:
		LOG_ERR("Unhandled Wi-Fi event: %llu", mgmt_event);
		break;
	}
}

static int connect_to_wifi(void)
{
	LOG_INF("Entered %s", __func__);

	if (!sta_iface)
	{
		LOG_ERR("STA: interface no initialized");
		return -EIO;
	}

	sta_config.ssid = (const uint8_t *)CONFIG_WIFI_SAMPLE_SSID;
	sta_config.ssid_length = sizeof(CONFIG_WIFI_SAMPLE_SSID) - 1;
	sta_config.psk = (const uint8_t *)CONFIG_WIFI_SAMPLE_PSK;
	sta_config.psk_length = sizeof(CONFIG_WIFI_SAMPLE_PSK) - 1;
	sta_config.security = WIFI_SECURITY_TYPE_PSK;
	sta_config.channel = WIFI_CHANNEL_ANY;
	sta_config.band = WIFI_FREQ_BAND_2_4_GHZ;

	LOG_INF("Connecting to SSID: %s\n", sta_config.ssid);

	const int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, sta_iface, &sta_config,
							 sizeof(struct wifi_connect_req_params));
	if (ret)
	{
		LOG_ERR("Unable to Connect to (%s)", CONFIG_WIFI_SAMPLE_SSID);
	}

	return ret;
}

int main(void)
{
	net_mgmt_init_event_callback(&cb, on_wifi_mgmt_event, NET_EVENT_WIFI_MASK);
	net_mgmt_add_event_callback(&cb);

	sta_iface = net_if_get_wifi_sta();

	connect_to_wifi();

	return 0;
}
