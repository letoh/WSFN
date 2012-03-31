CFLAGS  = `pkg-config --cflags cairo`
LDFLAGS = `pkg-config --libs cairo`

BIN = a b c

all: $(BIN)

clean:
	rm -f $(BIN)

