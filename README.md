# JSONStats

Takes a JSON file and reports statistics about that JSON file.

## Usage

```
Usage bin/jsonstats <json-file>
```

## How To Clone This Repository

```
git clone https://github.com/yavuzkoroglu/jsonstats.git
cd jsonstats
```

## How To Build

```
make
```

## Example Usage(`<json-file>` = example/example4.json)

```
cat example/example4.json; bin/jsonstats example/example4.json
```

## Example Output(`<json-file>` = example/example4.json)

```
{
    "grades": [ 4.00, 3.25, 2.56, 3.62 ],
    "courses": {
        "mandatory": [ "CE101", "CE102" ],
        "selective": [ "HUM101", "PHYS101" ]
    }
}

        # Tokens = 33
       # Objects = 2
        # Arrays = 3
       # Strings = 8
         # Names = 4
 # String Values = 4
      # Booleans = 0 (0 falses, 0 trues)
         # Nulls = 0
       # Numbers = 4
        # Values = 8
     # Variables = 12

       Max Depth = 3
  Max String Len = 9

Avg. All Numbers = 3.36

```
