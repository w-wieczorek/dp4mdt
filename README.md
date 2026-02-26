# dp4mdt

Decision tree generation from decision tables using an ILP-based test selection and dynamic programming.

## Installation

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler (make sure 17 is set in a Conan's preset file)
- Conan 2.x

### Build

1. Install dependencies and generate CMake toolchain/presets:

```bash
conan install . --output-folder=build --build=missing
```

2. Configure and build:

```bash
cmake --preset conan-default
cmake --build --preset conan-release
```

## Usage

Run the executable with training and test datasets:

```bash
./build/Release/dp4mdt --tre <training_file.csv> --test <test_file.csv> [--verbosity info|debug]
```

CLI options:

- `-t, --tre` (required): training data file
- `-e, --test` (required): test data file
- `-v, --verbosity` (optional): `info` (default) or `debug`

## Examples

Using sample benchmark datasets from the `benchmarks` directory:

```bash
./build/Release/dp4mdt --tre benchmarks/zoo_trte.data --test benchmarks/zoo_clean.data
```

Debug run:

```bash
./build/Release/dp4mdt --tre benchmarks/zoo_trte.data --test benchmarks/zoo_clean.data --verbosity debug
```

Expected output includes:

- training/test row counts
- generated decision tree height and size
- target and predicted class vectors

## Authors

Contributors names and contact info:

- [Wojciech Wieczorek](https://kiia.ubb.edu.pl/pracownicy/dr-habwojciechwieczorek),
- [Łukasz Strąk](https://ab.us.edu.pl/emp?id=47011),
- [Arkadiusz Nowakowski](https://ab.us.edu.pl/emp?id=46971).

## Licence

This project is available under Apache License 2.0.
