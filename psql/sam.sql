create table songs(
	song	varchar(255),
	rating	int,
	genre	varchar(255)
);

insert into songs values ('Your Reality', 9, 'Piano');
insert into songs values ('Play It Again', 10,'Country');
insert into songs values ('Marry You', 9,'Pop');
insert into songs values ('Carry On My Wayward Son', 7,'Classic Rock');
inster into songs values ('Uptown Funk', 4, 'Pop');

select * from songs;
select * from songs where genre='Country';

update song set rating=8 where song='Your Reality';

select * from songs;

delete from songs where song='Uptown Funk';

select * from songs;

create table test(
	name varchar(255),
	age int
);

insert into test values ('Dean Winchester', 32);
insert into test values ('Sam Winchester', 40);

select * from test;
delete test;