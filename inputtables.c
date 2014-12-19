#include <string.h>
#include <stdio.h>
#include <linux/input.h>

#define ELEMENT(NAME)	[NAME] = #NAME

static const char *evnames[EV_CNT] = {
	[EV_SYN] = "SYN",
	[EV_KEY] = "KEY",
	[EV_REL] = "REL",
	[EV_ABS] = "ABS",
	[EV_MSC] = "MSC",
	[EV_SW] = "SW",
	[EV_LED] = "LED",
	[EV_SND] = "SND",
	[EV_REP] = "REP",
	[EV_FF] = "FF",
	[EV_PWR] = "PWR",
};

#ifndef SYN_CNT
#define SYN_CNT 4
#endif
static const char *synnames[SYN_CNT] = {
	ELEMENT(SYN_REPORT),
	ELEMENT(SYN_CONFIG),
	ELEMENT(SYN_MT_REPORT),
	ELEMENT(SYN_DROPPED),
};

static const char *keynames[KEY_CNT] = {
	ELEMENT(KEY_RESERVED),
	ELEMENT(KEY_ESC),
	ELEMENT(KEY_1),
	ELEMENT(KEY_2),
	ELEMENT(KEY_3),
	ELEMENT(KEY_4),
	ELEMENT(KEY_5),
	ELEMENT(KEY_6),
	ELEMENT(KEY_7),
	ELEMENT(KEY_8),
	ELEMENT(KEY_9),
	ELEMENT(KEY_0),
	ELEMENT(KEY_MINUS),
	ELEMENT(KEY_EQUAL),
	ELEMENT(KEY_BACKSPACE),
	ELEMENT(KEY_TAB),
	ELEMENT(KEY_Q),
	ELEMENT(KEY_W),
	ELEMENT(KEY_E),
	ELEMENT(KEY_R),
	ELEMENT(KEY_T),
	ELEMENT(KEY_Y),
	ELEMENT(KEY_U),
	ELEMENT(KEY_I),
	ELEMENT(KEY_O),
	ELEMENT(KEY_P),
	ELEMENT(KEY_LEFTBRACE),
	ELEMENT(KEY_RIGHTBRACE),
	ELEMENT(KEY_ENTER),
	ELEMENT(KEY_LEFTCTRL),
	ELEMENT(KEY_A),
	ELEMENT(KEY_S),
	ELEMENT(KEY_D),
	ELEMENT(KEY_F),
	ELEMENT(KEY_G),
	ELEMENT(KEY_H),
	ELEMENT(KEY_J),
	ELEMENT(KEY_K),
	ELEMENT(KEY_L),
	ELEMENT(KEY_SEMICOLON),
	ELEMENT(KEY_APOSTROPHE),
	ELEMENT(KEY_GRAVE),
	ELEMENT(KEY_LEFTSHIFT),
	ELEMENT(KEY_BACKSLASH),
	ELEMENT(KEY_Z),
	ELEMENT(KEY_X),
	ELEMENT(KEY_C),
	ELEMENT(KEY_V),
	ELEMENT(KEY_B),
	ELEMENT(KEY_N),
	ELEMENT(KEY_M),
	ELEMENT(KEY_COMMA),
	ELEMENT(KEY_DOT),
	ELEMENT(KEY_SLASH),
	ELEMENT(KEY_RIGHTSHIFT),
	ELEMENT(KEY_KPASTERISK),
	ELEMENT(KEY_LEFTALT),
	ELEMENT(KEY_SPACE),
	ELEMENT(KEY_CAPSLOCK),
	ELEMENT(KEY_F1),
	ELEMENT(KEY_F2),
	ELEMENT(KEY_F3),
	ELEMENT(KEY_F4),
	ELEMENT(KEY_F5),
	ELEMENT(KEY_F6),
	ELEMENT(KEY_F7),
	ELEMENT(KEY_F8),
	ELEMENT(KEY_F9),
	ELEMENT(KEY_F10),
	ELEMENT(KEY_NUMLOCK),
	ELEMENT(KEY_SCROLLLOCK),
	ELEMENT(KEY_KP7),
	ELEMENT(KEY_KP8),
	ELEMENT(KEY_KP9),
	ELEMENT(KEY_KPMINUS),
	ELEMENT(KEY_KP4),
	ELEMENT(KEY_KP5),
	ELEMENT(KEY_KP6),
	ELEMENT(KEY_KPPLUS),
	ELEMENT(KEY_KP1),
	ELEMENT(KEY_KP2),
	ELEMENT(KEY_KP3),
	ELEMENT(KEY_KP0),
	ELEMENT(KEY_KPDOT),

	ELEMENT(KEY_ZENKAKUHANKAKU),
	ELEMENT(KEY_102ND),
	ELEMENT(KEY_F11),
	ELEMENT(KEY_F12),
	ELEMENT(KEY_RO),
	ELEMENT(KEY_KATAKANA),
	ELEMENT(KEY_HIRAGANA),
	ELEMENT(KEY_HENKAN),
	ELEMENT(KEY_KATAKANAHIRAGANA),
	ELEMENT(KEY_MUHENKAN),
	ELEMENT(KEY_KPJPCOMMA),
	ELEMENT(KEY_KPENTER),
	ELEMENT(KEY_RIGHTCTRL),
	ELEMENT(KEY_KPSLASH),
	ELEMENT(KEY_SYSRQ),
	ELEMENT(KEY_RIGHTALT),
	ELEMENT(KEY_LINEFEED),
	ELEMENT(KEY_HOME),
	ELEMENT(KEY_UP),
	ELEMENT(KEY_PAGEUP),
	ELEMENT(KEY_LEFT),
	ELEMENT(KEY_RIGHT),
	ELEMENT(KEY_END),
	ELEMENT(KEY_DOWN),
	ELEMENT(KEY_PAGEDOWN),
	ELEMENT(KEY_INSERT),
	ELEMENT(KEY_DELETE),
	ELEMENT(KEY_MACRO),
	ELEMENT(KEY_MUTE),
	ELEMENT(KEY_VOLUMEDOWN),
	ELEMENT(KEY_VOLUMEUP),
	ELEMENT(KEY_POWER),
	ELEMENT(KEY_KPEQUAL),
	ELEMENT(KEY_KPPLUSMINUS),
	ELEMENT(KEY_PAUSE),
	ELEMENT(KEY_SCALE),

	ELEMENT(KEY_KPCOMMA),
	ELEMENT(KEY_HANGEUL),
	ELEMENT(KEY_HANGUEL),
	ELEMENT(KEY_HANJA),
	ELEMENT(KEY_YEN),
	ELEMENT(KEY_LEFTMETA),
	ELEMENT(KEY_RIGHTMETA),
	ELEMENT(KEY_COMPOSE),

	ELEMENT(KEY_STOP),
	ELEMENT(KEY_AGAIN),
	ELEMENT(KEY_PROPS),
	ELEMENT(KEY_UNDO),
	ELEMENT(KEY_FRONT),
	ELEMENT(KEY_COPY),
	ELEMENT(KEY_OPEN),
	ELEMENT(KEY_PASTE),
	ELEMENT(KEY_FIND),
	ELEMENT(KEY_CUT),
	ELEMENT(KEY_HELP),
	ELEMENT(KEY_MENU),
	ELEMENT(KEY_CALC),
	ELEMENT(KEY_SETUP),
	ELEMENT(KEY_SLEEP),
	ELEMENT(KEY_WAKEUP),
	ELEMENT(KEY_FILE),
	ELEMENT(KEY_SENDFILE),
	ELEMENT(KEY_DELETEFILE),
	ELEMENT(KEY_XFER),
	ELEMENT(KEY_PROG1),
	ELEMENT(KEY_PROG2),
	ELEMENT(KEY_WWW),
	ELEMENT(KEY_MSDOS),
	ELEMENT(KEY_COFFEE),
	ELEMENT(KEY_SCREENLOCK),
	ELEMENT(KEY_DIRECTION),
	ELEMENT(KEY_CYCLEWINDOWS),
	ELEMENT(KEY_MAIL),
	ELEMENT(KEY_BOOKMARKS),
	ELEMENT(KEY_COMPUTER),
	ELEMENT(KEY_BACK),
	ELEMENT(KEY_FORWARD),
	ELEMENT(KEY_CLOSECD),
	ELEMENT(KEY_EJECTCD),
	ELEMENT(KEY_EJECTCLOSECD),
	ELEMENT(KEY_NEXTSONG),
	ELEMENT(KEY_PLAYPAUSE),
	ELEMENT(KEY_PREVIOUSSONG),
	ELEMENT(KEY_STOPCD),
	ELEMENT(KEY_RECORD),
	ELEMENT(KEY_REWIND),
	ELEMENT(KEY_PHONE),
	ELEMENT(KEY_ISO),
	ELEMENT(KEY_CONFIG),
	ELEMENT(KEY_HOMEPAGE),
	ELEMENT(KEY_REFRESH),
	ELEMENT(KEY_EXIT),
	ELEMENT(KEY_MOVE),
	ELEMENT(KEY_EDIT),
	ELEMENT(KEY_SCROLLUP),
	ELEMENT(KEY_SCROLLDOWN),
	ELEMENT(KEY_KPLEFTPAREN),
	ELEMENT(KEY_KPRIGHTPAREN),
	ELEMENT(KEY_NEW),
	ELEMENT(KEY_REDO),

	ELEMENT(KEY_F13),
	ELEMENT(KEY_F14),
	ELEMENT(KEY_F15),
	ELEMENT(KEY_F16),
	ELEMENT(KEY_F17),
	ELEMENT(KEY_F18),
	ELEMENT(KEY_F19),
	ELEMENT(KEY_F20),
	ELEMENT(KEY_F21),
	ELEMENT(KEY_F22),
	ELEMENT(KEY_F23),
	ELEMENT(KEY_F24),

	ELEMENT(KEY_PLAYCD),
	ELEMENT(KEY_PAUSECD),
	ELEMENT(KEY_PROG3),
	ELEMENT(KEY_PROG4),
	ELEMENT(KEY_DASHBOARD),
	ELEMENT(KEY_SUSPEND),
	ELEMENT(KEY_CLOSE),
	ELEMENT(KEY_PLAY),
	ELEMENT(KEY_FASTFORWARD),
	ELEMENT(KEY_BASSBOOST),
	ELEMENT(KEY_PRINT),
	ELEMENT(KEY_HP),
	ELEMENT(KEY_CAMERA),
	ELEMENT(KEY_SOUND),
	ELEMENT(KEY_QUESTION),
	ELEMENT(KEY_EMAIL),
	ELEMENT(KEY_CHAT),
	ELEMENT(KEY_SEARCH),
	ELEMENT(KEY_CONNECT),
	ELEMENT(KEY_FINANCE),
	ELEMENT(KEY_SPORT),
	ELEMENT(KEY_SHOP),
	ELEMENT(KEY_ALTERASE),
	ELEMENT(KEY_CANCEL),
	ELEMENT(KEY_BRIGHTNESSDOWN),
	ELEMENT(KEY_BRIGHTNESSUP),
	ELEMENT(KEY_MEDIA),

	ELEMENT(KEY_SWITCHVIDEOMODE),

	ELEMENT(KEY_KBDILLUMTOGGLE),
	ELEMENT(KEY_KBDILLUMDOWN),
	ELEMENT(KEY_KBDILLUMUP),

	ELEMENT(KEY_SEND),
	ELEMENT(KEY_REPLY),
	ELEMENT(KEY_FORWARDMAIL),
	ELEMENT(KEY_SAVE),
	ELEMENT(KEY_DOCUMENTS),

	ELEMENT(KEY_BATTERY),

	ELEMENT(KEY_BLUETOOTH),
	ELEMENT(KEY_WLAN),
	ELEMENT(KEY_UWB),

	ELEMENT(KEY_UNKNOWN),

	ELEMENT(KEY_VIDEO_NEXT),
	ELEMENT(KEY_VIDEO_PREV),
	ELEMENT(KEY_BRIGHTNESS_CYCLE),
	ELEMENT(KEY_BRIGHTNESS_ZERO),
	ELEMENT(KEY_DISPLAY_OFF),

	ELEMENT(KEY_WIMAX),
	ELEMENT(KEY_RFKILL),

	ELEMENT(KEY_MICMUTE),

	ELEMENT(BTN_MISC),
	ELEMENT(BTN_0),
	ELEMENT(BTN_1),
	ELEMENT(BTN_2),
	ELEMENT(BTN_3),
	ELEMENT(BTN_4),
	ELEMENT(BTN_5),
	ELEMENT(BTN_6),
	ELEMENT(BTN_7),
	ELEMENT(BTN_8),
	ELEMENT(BTN_9),

	ELEMENT(BTN_MOUSE),
	ELEMENT(BTN_LEFT),
	ELEMENT(BTN_RIGHT),
	ELEMENT(BTN_MIDDLE),
	ELEMENT(BTN_SIDE),
	ELEMENT(BTN_EXTRA),
	ELEMENT(BTN_FORWARD),
	ELEMENT(BTN_BACK),
	ELEMENT(BTN_TASK),

	ELEMENT(BTN_JOYSTICK),
	ELEMENT(BTN_TRIGGER),
	ELEMENT(BTN_THUMB),
	ELEMENT(BTN_THUMB2),
	ELEMENT(BTN_TOP),
	ELEMENT(BTN_TOP2),
	ELEMENT(BTN_PINKIE),
	ELEMENT(BTN_BASE),
	ELEMENT(BTN_BASE2),
	ELEMENT(BTN_BASE3),
	ELEMENT(BTN_BASE4),
	ELEMENT(BTN_BASE5),
	ELEMENT(BTN_BASE6),
	ELEMENT(BTN_DEAD),

	ELEMENT(BTN_GAMEPAD),
	ELEMENT(BTN_A),
	ELEMENT(BTN_B),
	ELEMENT(BTN_C),
	ELEMENT(BTN_X),
	ELEMENT(BTN_Y),
	ELEMENT(BTN_Z),
	ELEMENT(BTN_TL),
	ELEMENT(BTN_TR),
	ELEMENT(BTN_TL2),
	ELEMENT(BTN_TR2),
	ELEMENT(BTN_SELECT),
	ELEMENT(BTN_START),
	ELEMENT(BTN_MODE),
	ELEMENT(BTN_THUMBL),
	ELEMENT(BTN_THUMBR),

	ELEMENT(BTN_DIGI),
	ELEMENT(BTN_TOOL_PEN),
	ELEMENT(BTN_TOOL_RUBBER),
	ELEMENT(BTN_TOOL_BRUSH),
	ELEMENT(BTN_TOOL_PENCIL),
	ELEMENT(BTN_TOOL_AIRBRUSH),
	ELEMENT(BTN_TOOL_FINGER),
	ELEMENT(BTN_TOOL_MOUSE),
	ELEMENT(BTN_TOOL_LENS),
	ELEMENT(BTN_TOOL_QUINTTAP),
	ELEMENT(BTN_TOUCH),
	ELEMENT(BTN_STYLUS),
	ELEMENT(BTN_STYLUS2),
	ELEMENT(BTN_TOOL_DOUBLETAP),
	ELEMENT(BTN_TOOL_TRIPLETAP),
	ELEMENT(BTN_TOOL_QUADTAP),

	ELEMENT(BTN_WHEEL),
	ELEMENT(BTN_GEAR_DOWN),
	ELEMENT(BTN_GEAR_UP),

	ELEMENT(KEY_OK),
	ELEMENT(KEY_SELECT),
	ELEMENT(KEY_GOTO),
	ELEMENT(KEY_CLEAR),
	ELEMENT(KEY_POWER2),
	ELEMENT(KEY_OPTION),
	ELEMENT(KEY_INFO),
	ELEMENT(KEY_TIME),
	ELEMENT(KEY_VENDOR),
	ELEMENT(KEY_ARCHIVE),
	ELEMENT(KEY_PROGRAM),
	ELEMENT(KEY_CHANNEL),
	ELEMENT(KEY_FAVORITES),
	ELEMENT(KEY_EPG),
	ELEMENT(KEY_PVR),
	ELEMENT(KEY_MHP),
	ELEMENT(KEY_LANGUAGE),
	ELEMENT(KEY_TITLE),
	ELEMENT(KEY_SUBTITLE),
	ELEMENT(KEY_ANGLE),
	ELEMENT(KEY_ZOOM),
	ELEMENT(KEY_MODE),
	ELEMENT(KEY_KEYBOARD),
	ELEMENT(KEY_SCREEN),
	ELEMENT(KEY_PC),
	ELEMENT(KEY_TV),
	ELEMENT(KEY_TV2),
	ELEMENT(KEY_VCR),
	ELEMENT(KEY_VCR2),
	ELEMENT(KEY_SAT),
	ELEMENT(KEY_SAT2),
	ELEMENT(KEY_CD),
	ELEMENT(KEY_TAPE),
	ELEMENT(KEY_RADIO),
	ELEMENT(KEY_TUNER),
	ELEMENT(KEY_PLAYER),
	ELEMENT(KEY_TEXT),
	ELEMENT(KEY_DVD),
	ELEMENT(KEY_AUX),
	ELEMENT(KEY_MP3),
	ELEMENT(KEY_AUDIO),
	ELEMENT(KEY_VIDEO),
	ELEMENT(KEY_DIRECTORY),
	ELEMENT(KEY_LIST),
	ELEMENT(KEY_MEMO),
	ELEMENT(KEY_CALENDAR),
	ELEMENT(KEY_RED),
	ELEMENT(KEY_GREEN),
	ELEMENT(KEY_YELLOW),
	ELEMENT(KEY_BLUE),
	ELEMENT(KEY_CHANNELUP),
	ELEMENT(KEY_CHANNELDOWN),
	ELEMENT(KEY_FIRST),
	ELEMENT(KEY_LAST),
	ELEMENT(KEY_AB),
	ELEMENT(KEY_NEXT),
	ELEMENT(KEY_RESTART),
	ELEMENT(KEY_SLOW),
	ELEMENT(KEY_SHUFFLE),
	ELEMENT(KEY_BREAK),
	ELEMENT(KEY_PREVIOUS),
	ELEMENT(KEY_DIGITS),
	ELEMENT(KEY_TEEN),
	ELEMENT(KEY_TWEN),
	ELEMENT(KEY_VIDEOPHONE),
	ELEMENT(KEY_GAMES),
	ELEMENT(KEY_ZOOMIN),
	ELEMENT(KEY_ZOOMOUT),
	ELEMENT(KEY_ZOOMRESET),
	ELEMENT(KEY_WORDPROCESSOR),
	ELEMENT(KEY_EDITOR),
	ELEMENT(KEY_SPREADSHEET),
	ELEMENT(KEY_GRAPHICSEDITOR),
	ELEMENT(KEY_PRESENTATION),
	ELEMENT(KEY_DATABASE),
	ELEMENT(KEY_NEWS),
	ELEMENT(KEY_VOICEMAIL),
	ELEMENT(KEY_ADDRESSBOOK),
	ELEMENT(KEY_MESSENGER),
	ELEMENT(KEY_DISPLAYTOGGLE),
	ELEMENT(KEY_SPELLCHECK),
	ELEMENT(KEY_LOGOFF),

	ELEMENT(KEY_DOLLAR),
	ELEMENT(KEY_EURO),

	ELEMENT(KEY_FRAMEBACK),
	ELEMENT(KEY_FRAMEFORWARD),
	ELEMENT(KEY_CONTEXT_MENU),
	ELEMENT(KEY_MEDIA_REPEAT),
	ELEMENT(KEY_10CHANNELSUP),
	ELEMENT(KEY_10CHANNELSDOWN),
	ELEMENT(KEY_IMAGES),

	ELEMENT(KEY_DEL_EOL),
	ELEMENT(KEY_DEL_EOS),
	ELEMENT(KEY_INS_LINE),
	ELEMENT(KEY_DEL_LINE),

	ELEMENT(KEY_FN),
	ELEMENT(KEY_FN_ESC),
	ELEMENT(KEY_FN_F1),
	ELEMENT(KEY_FN_F2),
	ELEMENT(KEY_FN_F3),
	ELEMENT(KEY_FN_F4),
	ELEMENT(KEY_FN_F5),
	ELEMENT(KEY_FN_F6),
	ELEMENT(KEY_FN_F7),
	ELEMENT(KEY_FN_F8),
	ELEMENT(KEY_FN_F9),
	ELEMENT(KEY_FN_F10),
	ELEMENT(KEY_FN_F11),
	ELEMENT(KEY_FN_F12),
	ELEMENT(KEY_FN_1),
	ELEMENT(KEY_FN_2),
	ELEMENT(KEY_FN_D),
	ELEMENT(KEY_FN_E),
	ELEMENT(KEY_FN_F),
	ELEMENT(KEY_FN_S),
	ELEMENT(KEY_FN_B),

	ELEMENT(KEY_BRL_DOT1),
	ELEMENT(KEY_BRL_DOT2),
	ELEMENT(KEY_BRL_DOT3),
	ELEMENT(KEY_BRL_DOT4),
	ELEMENT(KEY_BRL_DOT5),
	ELEMENT(KEY_BRL_DOT6),
	ELEMENT(KEY_BRL_DOT7),
	ELEMENT(KEY_BRL_DOT8),
	ELEMENT(KEY_BRL_DOT9),
	ELEMENT(KEY_BRL_DOT10),

	ELEMENT(KEY_NUMERIC_0),
	ELEMENT(KEY_NUMERIC_1),
	ELEMENT(KEY_NUMERIC_2),
	ELEMENT(KEY_NUMERIC_3),
	ELEMENT(KEY_NUMERIC_4),
	ELEMENT(KEY_NUMERIC_5),
	ELEMENT(KEY_NUMERIC_6),
	ELEMENT(KEY_NUMERIC_7),
	ELEMENT(KEY_NUMERIC_8),
	ELEMENT(KEY_NUMERIC_9),
	ELEMENT(KEY_NUMERIC_STAR),
	ELEMENT(KEY_NUMERIC_POUND),

	ELEMENT(KEY_CAMERA_FOCUS),
	ELEMENT(KEY_WPS_BUTTON),

	ELEMENT(KEY_TOUCHPAD_TOGGLE),
	ELEMENT(KEY_TOUCHPAD_ON),
	ELEMENT(KEY_TOUCHPAD_OFF),

	ELEMENT(KEY_CAMERA_ZOOMIN),
	ELEMENT(KEY_CAMERA_ZOOMOUT),
	ELEMENT(KEY_CAMERA_UP),
	ELEMENT(KEY_CAMERA_DOWN),
	ELEMENT(KEY_CAMERA_LEFT),
	ELEMENT(KEY_CAMERA_RIGHT),

	ELEMENT(BTN_TRIGGER_HAPPY),
	ELEMENT(BTN_TRIGGER_HAPPY1),
	ELEMENT(BTN_TRIGGER_HAPPY2),
	ELEMENT(BTN_TRIGGER_HAPPY3),
	ELEMENT(BTN_TRIGGER_HAPPY4),
	ELEMENT(BTN_TRIGGER_HAPPY5),
	ELEMENT(BTN_TRIGGER_HAPPY6),
	ELEMENT(BTN_TRIGGER_HAPPY7),
	ELEMENT(BTN_TRIGGER_HAPPY8),
	ELEMENT(BTN_TRIGGER_HAPPY9),
	ELEMENT(BTN_TRIGGER_HAPPY10),
	ELEMENT(BTN_TRIGGER_HAPPY11),
	ELEMENT(BTN_TRIGGER_HAPPY12),
	ELEMENT(BTN_TRIGGER_HAPPY13),
	ELEMENT(BTN_TRIGGER_HAPPY14),
	ELEMENT(BTN_TRIGGER_HAPPY15),
	ELEMENT(BTN_TRIGGER_HAPPY16),
	ELEMENT(BTN_TRIGGER_HAPPY17),
	ELEMENT(BTN_TRIGGER_HAPPY18),
	ELEMENT(BTN_TRIGGER_HAPPY19),
	ELEMENT(BTN_TRIGGER_HAPPY20),
	ELEMENT(BTN_TRIGGER_HAPPY21),
	ELEMENT(BTN_TRIGGER_HAPPY22),
	ELEMENT(BTN_TRIGGER_HAPPY23),
	ELEMENT(BTN_TRIGGER_HAPPY24),
	ELEMENT(BTN_TRIGGER_HAPPY25),
	ELEMENT(BTN_TRIGGER_HAPPY26),
	ELEMENT(BTN_TRIGGER_HAPPY27),
	ELEMENT(BTN_TRIGGER_HAPPY28),
	ELEMENT(BTN_TRIGGER_HAPPY29),
	ELEMENT(BTN_TRIGGER_HAPPY30),
	ELEMENT(BTN_TRIGGER_HAPPY31),
	ELEMENT(BTN_TRIGGER_HAPPY32),
	ELEMENT(BTN_TRIGGER_HAPPY33),
	ELEMENT(BTN_TRIGGER_HAPPY34),
	ELEMENT(BTN_TRIGGER_HAPPY35),
	ELEMENT(BTN_TRIGGER_HAPPY36),
	ELEMENT(BTN_TRIGGER_HAPPY37),
	ELEMENT(BTN_TRIGGER_HAPPY38),
	ELEMENT(BTN_TRIGGER_HAPPY39),
	ELEMENT(BTN_TRIGGER_HAPPY40),
};

static const char *relnames[REL_CNT] = {
	ELEMENT(REL_X),
	ELEMENT(REL_Y),
	ELEMENT(REL_Z),
	ELEMENT(REL_RX),
	ELEMENT(REL_RY),
	ELEMENT(REL_RZ),
	ELEMENT(REL_HWHEEL),
	ELEMENT(REL_DIAL),
	ELEMENT(REL_WHEEL),
	ELEMENT(REL_MISC),
};

static const char *absnames[ABS_CNT] = {
	ELEMENT(ABS_X),
	ELEMENT(ABS_Y),
	ELEMENT(ABS_Z),
	ELEMENT(ABS_RX),
	ELEMENT(ABS_RY),
	ELEMENT(ABS_RZ),
	ELEMENT(ABS_THROTTLE),
	ELEMENT(ABS_RUDDER),
	ELEMENT(ABS_WHEEL),
	ELEMENT(ABS_GAS),
	ELEMENT(ABS_BRAKE),
	ELEMENT(ABS_HAT0X),
	ELEMENT(ABS_HAT0Y),
	ELEMENT(ABS_HAT1X),
	ELEMENT(ABS_HAT1Y),
	ELEMENT(ABS_HAT2X),
	ELEMENT(ABS_HAT2Y),
	ELEMENT(ABS_HAT3X),
	ELEMENT(ABS_HAT3Y),
	ELEMENT(ABS_PRESSURE),
	ELEMENT(ABS_DISTANCE),
	ELEMENT(ABS_TILT_X),
	ELEMENT(ABS_TILT_Y),
	ELEMENT(ABS_TOOL_WIDTH),

	ELEMENT(ABS_VOLUME),

	ELEMENT(ABS_MISC),

	ELEMENT(ABS_MT_SLOT),
	ELEMENT(ABS_MT_TOUCH_MAJOR),
	ELEMENT(ABS_MT_TOUCH_MINOR),
	ELEMENT(ABS_MT_WIDTH_MAJOR),
	ELEMENT(ABS_MT_WIDTH_MINOR),
	ELEMENT(ABS_MT_ORIENTATION),
	ELEMENT(ABS_MT_POSITION_X),
	ELEMENT(ABS_MT_POSITION_Y),
	ELEMENT(ABS_MT_TOOL_TYPE),
	ELEMENT(ABS_MT_BLOB_ID),
	ELEMENT(ABS_MT_TRACKING_ID),
	ELEMENT(ABS_MT_PRESSURE),
	ELEMENT(ABS_MT_DISTANCE),
};

static const char *swnames[SW_CNT] = {
	ELEMENT(SW_LID),
	ELEMENT(SW_TABLET_MODE),
	ELEMENT(SW_HEADPHONE_INSERT),
	[SW_RFKILL_ALL] = "SW_RFKILL",
	ELEMENT(SW_MICROPHONE_INSERT),
	ELEMENT(SW_DOCK),
	ELEMENT(SW_LINEOUT_INSERT),
	ELEMENT(SW_JACK_PHYSICAL_INSERT),
	ELEMENT(SW_VIDEOOUT_INSERT),
	ELEMENT(SW_CAMERA_LENS_COVER),
	ELEMENT(SW_KEYPAD_SLIDE),
	ELEMENT(SW_FRONT_PROXIMITY),
	ELEMENT(SW_ROTATE_LOCK),
	ELEMENT(SW_LINEIN_INSERT),
};

static const char *mscnames[MSC_CNT] = {
	ELEMENT(MSC_SERIAL),
	ELEMENT(MSC_PULSELED),
	ELEMENT(MSC_GESTURE),
	ELEMENT(MSC_RAW),
	ELEMENT(MSC_SCAN),
};

static const char *lednames[LED_CNT] = {
	ELEMENT(LED_NUML),
	ELEMENT(LED_CAPSL),
	ELEMENT(LED_SCROLLL),
	ELEMENT(LED_COMPOSE),
	ELEMENT(LED_KANA),
	ELEMENT(LED_SLEEP),
	ELEMENT(LED_SUSPEND),
	ELEMENT(LED_MUTE),
	ELEMENT(LED_MISC),
	ELEMENT(LED_MAIL),
	ELEMENT(LED_CHARGING),
};

static const char *repnames[REP_CNT] = {
	ELEMENT(REP_DELAY),
	ELEMENT(REP_PERIOD),
};

static const char *sndnames[SND_CNT] = {
	ELEMENT(SND_CLICK),
	ELEMENT(SND_BELL),
	ELEMENT(SND_TONE),
};

static const char *const *typetables[EV_CNT] = {
	[EV_SYN] = synnames,
	[EV_KEY] = keynames,
	[EV_REL] = relnames,
	[EV_ABS] = absnames,
	[EV_SW] = swnames,
	[EV_MSC] = mscnames,
	[EV_LED] = lednames,
	[EV_REP] = repnames,
	[EV_SND] = sndnames,
};

#define SIZE(x)	(sizeof(x)/sizeof((x)[0]))
static int typemax[EV_CNT] = {
	[EV_SYN] = SIZE(synnames),
	[EV_KEY] = SIZE(keynames),
	[EV_REL] = SIZE(relnames),
	[EV_ABS] = SIZE(absnames),
	[EV_SW] = SIZE(swnames),
	[EV_MSC] = SIZE(mscnames),
	[EV_LED] = SIZE(lednames),
	[EV_REP] = SIZE(repnames),
	[EV_SND] = SIZE(sndnames),
};

const char *inputeventtostr(int type, int code)
{
	static char buf[32];

	sprintf(buf, "#%u:%u", type, code);

	if (type >= EV_CNT) {
		sprintf(buf, "#%u:%u", type, code);
		return buf;
	}
	sprintf(buf, "%s_%u", evnames[type], code);
	if (code >= typemax[type])
		return buf;
	return (typetables[type])[code] ?: buf;
}

int strtoinputevent(const char *str, int *ptype)
{
	const char *sep = strchr(str, '_');
	int j, type = EV_KEY /* try EV_KEY by default */;
	const char *const *table;

	if (!sep)
		return -1;
	/* detect type */
	for (j = 0; j < EV_CNT; ++j) {
		if (evnames[j] && !strncmp(evnames[j], str, sep - str)) {
			type = j;
			break;
		}
	}
	/* lookup in table */
	table = typetables[type];
	if (!table)
		return -1;
	for (j = 0; j < typemax[type]; ++j) {
		if (table[j] && !strcmp(table[j], str)) {
			if (ptype)
				*ptype = type;
			return j;
		}
	}
	return -1;
}