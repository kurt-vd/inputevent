# inputevent

This program read input events from a device and
emits them in readable text on stdout in the form TIME CODE VALUE

This makes it easy to pipe in a shell script like:

	while read TIME CODE VALUE; do
	case "$CODE" in
	KEY_PLAY)
		mpc play
		;;
	esac

To deploy easily, **inputevent** can spawn a child process that reads its output.
You can then simply put a single statement in your service monitoring.

**inputevent** can also emit the initial state of the input device.
This makes the boot procedure more predictable, you do not have to wait
for **inputevent** to start before pressing a button, or setting a switch.
