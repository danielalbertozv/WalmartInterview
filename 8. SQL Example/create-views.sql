-- *****    CREATE VIEWS  ******************************
CREATE VIEW view_locations AS       
SELECT l.name_ID, d.distance
FROM locations AS l JOIN distances AS d 
WHERE distance < 200;

-- Updateable views cannot have:
-- Aggregate functions
-- GROUP BY
-- HAVING
-- UNION
-- DISTINCT
-- LEFT RIGHT OUTER JOIN
-- SubQueries
