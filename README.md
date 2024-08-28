# JSONStats

Takes a JSON file and reports statistics about that JSON file.

## Usage

```
Usage bin/jsonstats <json-file>
```

## How To Clone Thie Repository

```
git clone https://github.com/yavuzkoroglu/jsonstats.git
cd jsonstats
```

## How To Build

```
make
```

## Example Usage(`<json-file>` = example/example1.json)

```
bin/jsonstats example/example1.json
```

## Example Output(`<json-file>` = example/example1.json)

```

        # Tokens = 353
       # Objects = 12
        # Arrays = 1
       # Strings = 134
         # Names = 81
 # String Values = 53
      # Booleans = 3 (1 falses, 2 trues)
         # Nulls = 0
       # Numbers = 18
        # Values = 74

       Max Depth = 5
  Max String Len = 59

```
