#ifndef _STUMBLER_H_
#define _STUMBLER_H_

#define CRYPT_NONE		0
#define CRYPT_WEP		1
#define CRYPT_WPA1		2
#define CRYPT_WPA		3
#define CRYPT_WPA1_TKIP		4
#define CRYPT_WPA1_TKIP_PSK	5
#define CRYPT_WPA1_CCMP		6
#define CRYPT_WPA1_CCMP_PSK	7
#define CRYPT_80211i		8
#define CRYPT_80211i_TKIP	9
#define CRYPT_80211i_TKIP_PSK	10
#define CRYPT_WPA_2		11

struct chan_info {
	int locked;
	int chan;
	struct ieee80211req ireq;
	struct timeval last_hop;
};

struct node_info {
	unsigned char mac[6];
	int signal;
	int noise;
	int max;
	unsigned char ssid[256];
	int chan;
	int wep;
	int pos;
	int ap;

	struct timeval seen;

	struct node_info* prev;
	struct node_info* next;
};

/* string functions */
char *mac2str(unsigned char*);
char *wep2str(int);
char *ssid2str(struct node_info*);

/* operation functions */
void save_state(void);
void cleanup(int);
void clean_crap(void);
void die(int, char*);
void user_input(void);

/* channel functions */
void display_chan(void);
void set_chan(int);
void setup_if(char *);

/* cruses functions */
void display_node(struct node_info *);
void update_node(struct node_info *);

/* packet functions */
void open_bpf(char *, int);
void bpf_input(void);
void get_beacon_info(unsigned char *, int, struct node_info *);
int get_packet_info(struct ieee80211_frame *, unsigned char *, int, struct node_info *);
void radiotap(unsigned char *, int);

#endif /*_STUMBLER_H_ */
