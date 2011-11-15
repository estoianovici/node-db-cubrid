# db-cubrid: CUBRID database bindings for Node.js #

For detailed information about this and other Node.js
database bindings visit the [Node.js db-cubrid homepage] [homepage].

## INSTALL ##

Before proceeding with installation, you need to have the libcascci
development libraries and include files. PATH to CUBRID install is
mandatory and it should be specified prior to installation if not
already set. For example:

```bash
$ export CUBRID=/home/user/CUBRID
```

Install with npm:

```bash
$ npm install db-cubrid
```

## QUICK START ##

```javascript
var cubrid = require('db-cubrid');
new cubrid.Database({
    hostname: 'localhost',
    user: 'dba',
    password: 'password',
    database: 'node'
}).connect(function(error) {
    if (error) {
        return console.log("CONNECTION ERROR: " + error);
    }

    this.query().select('*').from('users').execute(function(error, rows) {
        if (error) {
            return console.log('ERROR: ' + error);
        }
        console.log(rows.length + ' ROWS');
    });
});
```

## LICENSE ##

This module is released under the [MIT License] [license].

[homepage]: http://nodejsdb.org/db-cubrid
[license]: http://www.opensource.org/licenses/mit-license.php
