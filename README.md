# Tiny C Database

A minimal SQLite-style database implemented in C as a learning project.

## Build

```sh
make
```

This will generated two binaries in `build` directory, `db` and `tests`.


## Run 

Run the database by passing a filename:
```sh
./build/db mydb.db
```

## Tests

```sh
make
./build/tests
```


