-- brew uninstall mysql
-- brew uninstall mysql-client

CALL firstnameAndLastnameID('test', 'test');

INSERT INTO user (username, email, password_hash, firstname_id, lastname_id) VALUES ('test0', 'kek@gmail.com', '2342342', '1', '1');
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id) VALUES ('test1', 'kek@gmail.com', '2342342', '1', '1');
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id) VALUES ('test2', 'kek@gmail.com', '2342342', '1', '1');

UPDATE new_table SET name='lol' WHERE id=20;

DELETE FROM user WHERE id=1;

-- PROCEDURES

USE `byTickets`;
DROP procedure IF EXISTS `firstnameAndLastnameID`;
DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `firstnameAndLastnameID`(
	IN firstnameVal VARCHAR(64),
    IN lastnameVal VARCHAR(64)
)
BEGIN
	DECLARE firstnameID MEDIUMINT;
    DECLARE lastnameID MEDIUMINT;
	SELECT id INTO firstnameID FROM firstname WHERE firstname=firstnameVal;
    SELECT id INTO lastnameID FROM lastname WHERE lastname=lastnameVal;
    IF (firstnameID IS NULL) THEN 
		INSERT INTO firstname (firstname) VALUES (firstnameVal);
        SELECT id INTO firstnameID FROM firstname WHERE firstname=firstnameVal;
    END IF;
    IF (lastnameID IS NULL) THEN 
		INSERT INTO lastname (lastname) VALUES (lastnameVal);
        SELECT id INTO lastnameID FROM lastname WHERE lastname=lastnameVal;
    END IF;
    SELECT firstnameID, lastnameID;
END$$
DELIMITER ;


USE `byTickets`;
DROP procedure IF EXISTS `create_backup`;
DELIMITER $$
USE `byTickets`$$
CREATE PROCEDURE `create_backup` (
	IN id MEDIUMINT UNSIGNED,
    IN username VARCHAR(32),
    IN email VARCHAR(128),
    IN phone INT UNSIGNED,
    IN country_code TINYINT UNSIGNED,
    IN last_login TIMESTAMP,
    IN registration TIMESTAMP,
    IN default_card_id MEDIUMINT,
    IN password_hash BIGINT,
    IN firstname_id MEDIUMINT UNSIGNED,
    IN lastname_id MEDIUMINT UNSIGNED,
    IN role ENUM('admin', 'headmaster', 'manager', 'user')
)
BEGIN
	INSERT INTO backup SET
		user_id=id,
		username=username,
		email=email,
		phone=phone,
		country_code=country_code,
		last_login=last_login,
		registration=registration,
		default_card_id=default_card_id,
		password_hash=password_hash,
		firstname_id=firstname_id,
		lastname_id=lastname_id,
		role=role,
		`when`=NOW();
END$$
DELIMITER ;


USE `byTickets`;
DROP procedure IF EXISTS `restore_backup`;
DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `restore_backup`(
	IN username_arg VARCHAR(32)
)
BEGIN
	DECLARE backup_id INT UNSIGNED;
	DECLARE l_user_id MEDIUMINT UNSIGNED; 
    DECLARE l_username VARCHAR(32);
    DECLARE l_email VARCHAR(128);
    DECLARE l_phone INT UNSIGNED;
	DECLARE l_country_code TINYINT UNSIGNED;
    DECLARE l_last_login TIMESTAMP;
    DECLARE l_registration TIMESTAMP;
	DECLARE l_default_card_id MEDIUMINT;
    DECLARE l_password_hash BIGINT;
    DECLARE l_firstname_id MEDIUMINT UNSIGNED;
    DECLARE l_lastname_id MEDIUMINT UNSIGNED;
    DECLARE l_role ENUM('admin', 'headmaster', 'manager', 'user');
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	SELECT 
		id, user_id, username, email, phone, country_code, last_login, registration, default_card_id, 
		password_hash, firstname_id, lastname_id, role
	INTO 
		backup_id, l_user_id, l_username, l_email, l_phone, l_country_code, l_last_login, l_registration, l_default_card_id,
		l_password_hash, l_firstname_id, l_lastname_id, l_role
	FROM 
		backup
	WHERE 
		username=username_arg
	ORDER BY 
		`when` DESC
	LIMIT 1;
    IF backup_id IS NOT NULL THEN
        SET autocommit = 0;
		START TRANSACTION;
		IF EXISTS (SELECT * FROM user WHERE username=l_username FOR UPDATE) THEN
			UPDATE user SET 
				id=l_user_id, 
                username=l_username, 
                email=l_email, 
                phone=l_phone, 
                country_code=l_country_code, 
                last_login=l_last_login, 
                registration=l_registration, 
                default_card_id=l_default_card_id,
				password_hash=l_password_hash,
                firstname_id=l_firstname_id, 
                lastname_id=l_lastname_id, 
                role=l_role,
                needs_update_triggered=0
            WHERE 
				username=l_username;
		ELSE
			INSERT INTO user SET
				id=l_user_id, 
                username=l_username, 
                email=l_email, 
                phone=l_phone, 
                country_code=l_country_code, 
                last_login=l_last_login, 
                registration=l_registration, 
                default_card_id=l_default_card_id,
				password_hash=l_password_hash,
                firstname_id=l_firstname_id, 
                lastname_id=l_lastname_id, 
                role=l_role,
                needs_update_triggered=1;
        END IF;
        DELETE FROM backup WHERE id=backup_id;
        COMMIT;
    END IF;
END$$
DELIMITER ;

-- DELIMITER $$
-- CREATE DEFINER=CURRENT_USER PROCEDURE new_procedure (
-- 	IN num INT,
--     IN str VARCHAR(32),
--     IN dt TIMESTAMP
-- )
-- BEGIN
-- 	SELECT CONCAT(num, ". ", str, " - ", dt);
-- END$$
-- DELIMITER ;

-- CALL new_procedure(2, "HI", NOW());


-- TRIGGERS

DROP TRIGGER IF EXISTS `byTickets`.`user_AFTER_INSERT`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER = CURRENT_USER TRIGGER `byTickets`.`user_AFTER_INSERT` AFTER INSERT ON `user` FOR EACH ROW
BEGIN
	INSERT INTO insert_log SET
	username=NEW.username,
    email=NEW.email,
    user_id=NEW.id,
    last_login=NEW.last_login,
    registration=NEW.registration,
    password_hash=NEW.password_hash,
    role=NEW.role,
    `when`=NOW();
END$$
DELIMITER ;


DROP TRIGGER IF EXISTS `byTickets`.`user_BEFORE_UPDATE`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` TRIGGER `user_BEFORE_UPDATE` BEFORE UPDATE ON`user` FOR EACH ROW BEGIN
	INSERT INTO update_log SET
    username=NEW.username,
    email=NEW.email,
    user_id=NEW.id,
    last_login=NEW.last_login,
    registration=NEW.registration,
    password_hash=NEW.password_hash,
    role=NEW.role,
    `when`=NOW();
    IF NEW.needs_update_triggered=0 THEN
		SET NEW.needs_update_triggered=1;
	ELSE 
		CALL create_backup(
			OLD.id, 
			OLD.username, 
			OLD.email, 
			OLD.phone, 
			OLD.country_code, 
			OLD.last_login,
			OLD.registration,
			OLD.default_card_id,
			OLD.password_hash,
			OLD.firstname_id,
			OLD.lastname_id,
			OLD.role
		);
    END IF;
END$$
DELIMITER ;


DROP TRIGGER IF EXISTS `byTickets`.`user_AFTER_DELETE`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` TRIGGER `user_AFTER_DELETE` AFTER DELETE ON `user` FOR EACH ROW BEGIN
	CALL create_backup(
		OLD.id, 
		OLD.username, 
		OLD.email, 
        OLD.phone, 
        OLD.country_code, 
        OLD.last_login,
		OLD.registration,
		OLD.default_card_id,
		OLD.password_hash,
		OLD.firstname_id,
		OLD.lastname_id,
		OLD.role
	);
END$$
DELIMITER ;


DROP TRIGGER IF EXISTS `byTickets`.`login_AFTER_INSERT`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` TRIGGER `login_AFTER_INSERT` AFTER INSERT ON `login` FOR EACH ROW 
BEGIN
	IF (NEW.status='ok') THEN 
    UPDATE user SET 
    last_login=NOW()
    WHERE username=NEW.username;
    END IF;
END$$
DELIMITER ;



