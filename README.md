# binance_order_book

A Binance Order Book Example

## Requirements

- C++17 compiller. It was tested on Apple clang version 12.0.0
- Any OS. It was tested on macOS
- CMake

## Compilation

```bash
git clone https://github.com/ivan-volnov/binance_order_book.git
cd binance_order_book
mkdir build
cd build/
cmake ..
make
```

## Usage

```text
Usage: binance_order_book [options]

Optional arguments:
--help             show this help message and exit
--reverse          show Order Book in reverse order
--depth            limit Order Book depth (min 10)
--load             load Binance depth stream from jsonl file
```

Prepare the data file

```bash
bzcat binance-btcusdt-2020-11-09T11:24:00.75639.bz2 | sed 's/^[^{]*//;$s/$//' > binance-btcusdt.jsonl
```

Run the app using data file

```bash
./binance_order_book --depth 11 --load binance-btcusdt.jsonl
```

Type in another terminal

```bash
killall -SIGUSR1 binance_order_book
killall -SIGUSR1 binance_order_book
```

Run Unit tests:

```bash
./test/binance_order_book
```