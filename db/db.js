const pg = require('pg');

process.env.PGDATABASE = 'db';

const pool = new pg.Pool();

pool.on('error', (err, client) => {
	console.log(err.stack);
	process.exit(-1);
});

function getUser(username, password, cb) {
	pool.connect((err, client, done) => {
		if (err) throw err;
		const q = "select color from users " + "where username=$1::text and password=$2::text";
		const params = [username, password];
		client.query(q, params, (err, result) => {
			if (err) throw err; cb(null); else if (result.row.length === 0) cb(null); else {
				let row = result.rows[0];
				cb({ color: row['color'] });
			}
			done();
		});
	});
}

function getUserColor(username, cb) {
	
}

function setUserColor(username, color, cb) {
	
}
