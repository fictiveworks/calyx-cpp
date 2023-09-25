# Calyx: C++

C++ port of the Calyx generative grammar engine.

## Development

### Install build tools

See the [Xmake installation guide](https://xmake.io/#/guide/installation).

### Compile and link

```bash
xmake build -y
```

(The `-y` is to confirm installation of Catch2 for testing)

### Run test executable

Calyx C++ is tested using Catch2. 

```bash
xmake run test
```

### Build for release

TBD

### Generate Visual Studio IDE files

```bash
xmake project -k vs2017 -m "debug,release"
```

## License

Will be licensed MIT when open sourced.
