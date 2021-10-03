-- ******   QUERIES  *****************************          
SELECT * FROM locations;               -- Query an entire table
SELECT distance FROM distances;		   -- Query one column (distance) from table distances
SELECT * FROM locations LIMIT 2;       -- Query first two rows of tale
SELECT name_ID FROM locations; 	       -- Query column from table
SELECT name_ID AS 'City Name' FROM locations; -- Query and change displayed named
SELECT * FROM locations ORDER BY name_ID;          	-- Query by ascending order
SELECT * FROM locations ORDER BY name_ID DESC;		-- Query by descending order
SELECT * FROM distances					-- Query with conditional condition
WHERE distance < 200;
SELECT * FROM distances					-- Query with conditional condition
WHERE distance < 200;
SELECT * FROM distances					-- Query with conditional and AND, row must include string 'as'
WHERE distance < 200 AND id_source LIKE '%as%'; 
SELECT * FROM distances					-- Query with conditional and OR
WHERE distance < 100 OR id_source = 'El Paso'; 

SELECT * FROM distances					-- Query with conditional and AND, row must hace an a in the second character of id_source
WHERE distance < 200 AND id_source LIKE '_a%'; 

SELECT * FROM locations					-- Relational Query
JOIN distances ON locations.name_ID = distances.id_source ORDER BY name_ID;

SELECT * FROM locations					-- Relational Query - Left Join, includes all from left side even thought they do not have match
LEFT JOIN distances ON locations.name_ID = distances.id_source ORDER BY name_ID;

SELECT * FROM locations					-- Relational Query - Right Join, includes all from right side even thought they do not have match
RIGHT JOIN distances ON locations.name_ID = distances.id_source ORDER BY name_ID;

SELECT AVG(distance) FROM distances; 	-- Query to obtain average
SELECT SUM(distance) FROM distances; 	-- Query to obtain average
SELECT COUNT(distance) FROM distances; 	-- Query to obtain average

SELECT d.id_source AS City_Source, COUNT(l.name_ID) AS City
FROM distances AS d
LEFT JOIN locations AS l ON d.id_source = l.name_ID
GROUP BY d.id_source
HAVING id_source = 'El Paso';

SELECT * FROM locations           -- Query, name_ID must end in 'Vegas'
WHERE  name_ID REGEXP 'Vegas$';
SELECT * FROM locations           -- Query, name_ID must have 'Vegas' or 'Paso'
WHERE  name_ID REGEXP 'Vegas|Paso';
SELECT * FROM locations           -- Query, name_ID must start 'Vegas' or have 'Seattle'
WHERE  name_ID REGEXP '^Las|Seattle';
SELECT * FROM locations           -- Query, name_ID must a or s, followed an e
WHERE  name_ID REGEXP '[as]e';
SELECT * FROM locations           -- Query, name_ID must e followed by an a or s
WHERE  name_ID REGEXP 'e[as]';
SELECT * FROM locations           -- Query, name_ID must e followed by an a or s
WHERE  name_ID REGEXP '[a-h]';    -- Query, name_ID must a letter within a range a to h
-- --- REGEXP ------------------
-- ^ beginning
-- $ end
-- | logical or
-- [abcd] contain a character wihtin a set
-- [a-d] contain a character within a range