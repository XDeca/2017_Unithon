var express = require('express');
var router = express.Router();
var mysql = require('mysql');

var pool = mysql.createPool({
	host:'localhost',
	user:'root',
	password:'1234',
	database:'alarm'
});

/* GET home page. */
router.get('/', function(req, res,next) {
	pool.getConnection(function(error, con){
		if(error) throw error;

		con.query('select * from alarm', function(err, rows){
			if(err) throw err;

			if(rows[0].go == 1)
				res.render('index',{title: "STOP"});
			else
				res.render('index',{title: "START"});

			con.release();
		});
  });
});

router.get('/motor', function(req, res,next) {
	pool.getConnection(function(error, con){
		if(error) throw error;

		con.query('select * from alarm', function(err, rows){
			if(err) throw err;
			res.render('motor',{title: rows[0].go});//'test', rows:rows});
			con.release();
		});
  });
	//res.render('index',{title:'test'});
});

router.post('/chng', function(req, res) {
	pool.getConnection(function(error, con){
		if(error) throw error;

		con.query('select * from alarm', function(err, rows){
			if(err) throw err;

			if(rows[0].go == 1)
				con.query('update alarm set go = ? where idx=1',0);
			else	
				con.query('update alarm set go = ? where idx=1',1);

			con.release();
		});
  });

	return res.redirect('/');
});

module.exports = router;
