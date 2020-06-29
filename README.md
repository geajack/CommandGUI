# CommandGUI
This is a cross-platform (tested on Windows and Ubuntu) app which allows you to create graphical wrappers for complicated command line programs. Google the command line options once, instead of every time you have to use the program!

The program looks like this:

![https://i.imgur.com/TWrHZub.png](https://i.imgur.com/TWrHZub.png)

In order to get this graphical wrapper for `wget`, all I had to do was feed CommandGUI this JSON file:

```json
{
    "name": "wget",
    "templateString": "wget %recursive%[-r] %noparent%[-np] %norobots%[-e robots=off]\
                %user%[--user=%user%] %password%[--password=%password%]\
                %outdir%[-P \"%outdir%\"]\
                %cut%[-nH --cut-dirs=%cut%]\
                %url%",
    "variables": [
        {
            "name": "url",
            "label": "URL",
            "type": "string"
        },
        {
            "name": "user",
            "label": "Username",
            "type": "string"
        },
        {
            "name": "password",
            "label": "Password",
            "type": "string"
        },
        {
            "name": "recursive",
            "label": "Recursive",
            "type": "boolean"
        },
        {
            "name": "noparent",
            "label": "No parent",
            "type": "boolean",
            "default": true
        },
        {
            "name": "norobots",
            "label": "Ignore robots.txt",
            "type": "boolean"
        },
        {
            "name": "outdir",
            "label": "Output directory",
            "type": "folder",
            "default": "/home/jack/Downloads"
        },
        {
            "name": "cut",
            "label": "Cut dirs",
            "type": "string"
        }
    ]
}
```

## Building

To build for Qt, enter the `build` directory and run `qmake` followed by `make`.