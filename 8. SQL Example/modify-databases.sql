-- *****    MODIFY  ******************************
DROP DATABASE walmart_example;			-- Deletes database with name
DROP TABLE locations;					-- Deletes table with name
DROP TABLE distances;

ALTER TABLE locations                        -- Modify a table
ADD name_location VARCHAR(255) AFTER id;

ALTER TABLE distances                   -- Add a foreign key to table
ADD FOREIGN KEY (id_destination) REFERENCES locations (id);
DELETE FROM locations WHERE name_ID = 'Las Vegas';  -- Delete a specific row

INSERT INTO locations_distances	(name_ID, distance, id_source, id_destination)	-- Insert row into table locations
VALUES ( 'Houston', 100, 'Houston', 'Houston');

CREATE TABLE locations_distances AS    -- Create a new table based on the relational query
SELECT * FROM locations					
JOIN distances ON locations.name_ID = distances.id_source ORDER BY name_ID;

UPDATE locations_distances         -- Update a single row cell
SET distance = 150
WHERE id = '0';