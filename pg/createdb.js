const pg = require('pg');

process.env.PGDATABASE = 'pg';
process.env.PGUSER = 'postgres';
process.env.PGPASSWORD = 'joelord';

function selectAll(client, cb) {
  client.query("select * from test order by id", (err, result) => {
    if (err) throw err;
    result.rows.map((row, i) => {
      console.log('  ' + i + " " + row.id + ' ' + row.x);
    });
    cb();
  });
}

function updateRow(client, id, x, cb)
function deleteRow(client, id, cb)