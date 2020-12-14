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
        UPDATE user SET last_login=NOW() WHERE username=NEW.username;
    END IF;
END$$
DELIMITER ;

-- ALL USERS

-- CALL add_default_card("kek", "PRIOR", 1111111111, "2021-05-01");
-- CALL buy_ticket("lol", '3');
-- CALL see_available_tickets();
-- CALL see_my_tickets("lol");
-- CALL find_tickets_by_date("2020-05-20 16:00:00");
-- CALL find_tickets_by_movie("Bladerunner");
-- CALL find_tickets_by_cinema("Belarus");
-- CALL filter_tickets_by_movie("Joker");
-- CALL filter_tickets_by_cinema("Belarus");
-- CALL filter_tickets_by_date("2020-05-20 14:00:00");


-- HEADMASTER


-- CALL amount_of_tickets("lol");
-- CALL see_transactions("kek");
-- CALL see_sales("kek");


-- MANAGER

-- CALL amount_of_tickets("lol");


-- ADMIN

-- CALL cancel_order('kek', '4');
-- CALL edit_movie('lol', 2, "Bladerunner 2049", '18+', 'блокбастер', '2017', '3');


-- Процедуры

-- Возможность привязывать банковскую карту по-умолчанию (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `add_default_card`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `add_default_card`(
    IN username_arg VARCHAR(32),
    IN bankname_arg VARCHAR(64),
    IN card_number_arg BIGINT UNSIGNED,
    IN valid_date_arg DATE
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE card_id MEDIUMINT UNSIGNED;
    DECLARE d_bank_id SMALLINT UNSIGNED;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    
    IF valid_date_arg <= CURRENT_DATE() THEN
        SET error = "Card date is not valid";
        SELECT error;
        LEAVE exit_label;
    END IF;

    START TRANSACTION;
    SELECT default_card_id INTO card_id FROM user WHERE username=username_arg;
    IF card_id IS NULL THEN
        SELECT id INTO d_bank_id FROM bank WHERE name=bankname_arg;
        IF d_bank_id IS NULL THEN
            INSERT INTO bank SET name=bankname_arg;
            SELECT id INTO d_bank_id FROM bank WHERE name=bankname_arg;
        END IF;
        IF NOT EXISTS(SELECT * FROM card WHERE number=card_number_arg AND bank_id=d_bank_id) THEN
            INSERT INTO card SET 
                number=card_number_arg,
                valid_date=valid_date_arg,
                bank_id=d_bank_id;
        END IF;
        SELECT id INTO card_id FROM card WHERE number=card_number_arg AND bank_id=d_bank_id;
    END IF;
    UPDATE user SET default_card_id=card_id WHERE username=username_arg;
    COMMIT;
END$$

DELIMITER ;


-- Возможность покупки билетов (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `buy_ticket`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `buy_ticket`(
    IN username_arg VARCHAR(32),
    IN store_id_arg INT UNSIGNED
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    DECLARE card_id_var MEDIUMINT UNSIGNED;
    DECLARE cinema_id_var TINYINT UNSIGNED;
    DECLARE movie_id_var INT UNSIGNED;
    DECLARE ticket_id_var INT UNSIGNED;
    DECLARE ticket_price_var INT UNSIGNED;
    DECLARE transaction_id_var INT UNSIGNED;
    DECLARE sale_id_var INT UNSIGNED;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg;
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT default_card_id INTO card_id_var FROM user WHERE username=username_arg;
    IF card_id_var IS NULL THEN
        SET error = "NO CARD ASSIGNED TO ACCOUNT";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT sale_id INTO sale_id_var FROM store WHERE id=store_id_arg;
    IF sale_id_var IS NOT NULL THEN
        SET error = "TICKET ALREADY BOUGHT";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT 
        cinema_id, movie_id, ticket_id, price
    INTO 
        cinema_id_var, movie_id_var, ticket_id_var, ticket_price_var  
    FROM store WHERE id=store_id_arg;
    IF cinema_id_var IS NULL THEN
        SET error = "WRONG TICKET NUMBER";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    START TRANSACTION;
    INSERT INTO transaction SET order_sum=ticket_price_var, card_id=card_id_var;
    SELECT id INTO transaction_id_var FROM transaction 
        WHERE order_sum=ticket_price_var AND card_id=card_id_var ORDER BY id LIMIT 1;
    INSERT INTO sale SET sale_date=NOW(), transaction_id=transaction_id_var, user_id=user_id_var;
    SELECT id INTO sale_id_var FROM sale WHERE transaction_id=transaction_id_var AND user_id=user_id_var LIMIT 1;
    UPDATE store SET sale_id=sale_id_var WHERE id=store_id_arg;
    COMMIT;
END$$

DELIMITER ;


-- Возможность просмотра доступного количества билетов на складе (директор, менеджер);

USE `byTickets`;
DROP procedure IF EXISTS `amount_of_tickets`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `amount_of_tickets`(
    IN username_arg VARCHAR(32)
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE amount INT UNSIGNED;
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND (role='headmaster' OR role='manager');
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT COUNT(id) INTO amount FROM store WHERE sale_id IS NULL;
    SELECT amount; 
END$$

DELIMITER ;


-- Возможность просмотра транзакций (директор);

USE `byTickets`;
DROP procedure IF EXISTS `see_transactions`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `see_transactions`(
    IN username_arg VARCHAR(32)
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND role='headmaster';
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT 
        t.order_sum money_transfered, 
        c.number card_number, c.valid_date card_valid_until_date, 
        b.name bank_name
    FROM transaction t 
        JOIN card c ON t.card_id = c.id
        JOIN bank b ON c.bank_id = b.id; 
END$$

DELIMITER ;


-- Возможность просмотра продаж (директор);

USE `byTickets`;
DROP procedure IF EXISTS `see_sales`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `see_sales`(
    IN username_arg VARCHAR(32)
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND (role='headmaster' OR role='admin');
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT 
        s.id, s.sale_date date,
        t.order_sum money_transfered, 
        u.username from_user,
        c.number card_number, c.valid_date card_valid_until_date, 
        b.name bank_name
    FROM sale s
        JOIN transaction t ON s.transaction_id = t.id
        JOIN user u ON s.user_id = u.id
        JOIN card c ON t.card_id = c.id
        JOIN bank b ON c.bank_id = b.id; 
END$$

DELIMITER ;


-- Возможность просмотра доступных для покупки билетов (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `see_available_tickets`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `see_available_tickets`()
BEGIN
    SELECT 
        s.id, s.price ticket_price, s.start session_start_time,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id
    WHERE
        s.sale_id IS NULL; 
END$$
DELIMITER ;


-- Возможность просмотра купленных билетов (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `see_my_tickets`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `see_my_tickets`(
    IN username_arg VARCHAR(32)
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg;
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT 
        s.id, s.price ticket_price, s.start session_start_time,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id
        JOIN sale sa ON s.sale_id = sa.id
    WHERE
        s.sale_id IS NOT NULL AND sa.user_id=user_id_var;
END$$

DELIMITER ;


-- Поиск билетов по определенной дате (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `find_tickets_by_date`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `find_tickets_by_date`(
    IN date TIMESTAMP
)
BEGIN
    SELECT 
        s.id, s.price ticket_price, s.start session_start_time,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        s.start=date AND s.sale_id IS NULL;
END$$

DELIMITER ;


-- Поиск билетов на определённый фильм (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `find_tickets_by_movie`;

DELIMITER $$
USE `byTickets`$$
CREATE PROCEDURE `find_tickets_by_movie` (
    IN movie_arg VARCHAR(128)
)
BEGIN
    SELECT 
        s.id, s.price ticket_price, s.start session_start_time,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        m.name=movie_arg AND s.sale_id IS NULL;
END$$

DELIMITER ;


-- Поиск билетов по определённому кинотеатру (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `find_tickets_by_cinema`;

DELIMITER $$
USE `byTickets`$$
CREATE PROCEDURE `find_tickets_by_cinema` (
    IN cinema_name_arg VARCHAR(32)
)
BEGIN
    SELECT 
        s.id, s.price ticket_price, s.start session_start_time,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        c.name=cinema_name_arg AND s.sale_id IS NULL;
END$$

DELIMITER ;


-- Фильтрация билетов по кинотеатру (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `filter_tickets_by_cinema`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `filter_tickets_by_cinema`(
    IN cinema_name_arg VARCHAR(32)
)
BEGIN
    SELECT 
        c.name cinema,
        s.price ticket_price, s.start session_start_time,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        c.name=cinema_name_arg AND s.sale_id IS NULL;
END$$

DELIMITER ;


-- Фильтрация билетов по времени сеанса (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `filter_tickets_by_date`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `filter_tickets_by_date`(
    IN date TIMESTAMP
)
BEGIN
    SELECT 
        s.start session_start_time, s.price ticket_price,
        c.name cinema,
        m.name movie,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        s.start=date AND s.sale_id IS NULL;
END$$

DELIMITER ;


-- Фильтрация билетов по фильму (все пользователи);

USE `byTickets`;
DROP procedure IF EXISTS `filter_tickets_by_movie`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `filter_tickets_by_movie`(
    IN movie_arg VARCHAR(128)
)
BEGIN
    SELECT 
        m.name movie,
        s.price ticket_price, s.start session_start_time,
        c.name cinema,
        t.hall, t.row, t.seat
    FROM store s
        JOIN cinema c ON s.cinema_id = c.id
        JOIN movie m ON s.movie_id = m.id
        JOIN ticket t ON s.ticket_id = t.id 
    WHERE
        m.name=movie_arg AND s.sale_id IS NULL;
END$$

DELIMITER ;

-- Возможность отмены заказов пользователей (администратор);

USE `byTickets`;
DROP procedure IF EXISTS `cancel_order`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `cancel_order`(
    IN username_arg VARCHAR(32),
    IN sale_id_arg INT UNSIGNED
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE transaction_id_var INT UNSIGNED;
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND role='admin';
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    IF NOT EXISTS(SELECT * FROM sale WHERE id=sale_id_arg) THEN
        SET error = "NO SALE WITH THIS ID";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    START TRANSACTION;
    UPDATE store SET sale_id=NULL WHERE sale_id=sale_id_arg;
    SELECT transaction_id INTO transaction_id_var FROM sale WHERE id=sale_id_arg;
    DELETE FROM sale WHERE id=sale_id_arg;
    DELETE FROM transaction WHERE id=transaction_id_var;
    COMMIT;
END$$
DELIMITER ;


-- Возможность редактирования различной информации о фильмах (администратор);

USE `byTickets`;
DROP procedure IF EXISTS `edit_movie`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `edit_movie`(
    IN username_arg VARCHAR(32),
    IN movie_id_arg INT UNSIGNED,
    IN name_arg VARCHAR(128),
    IN movie_pg_arg ENUM('0+', '6+', '12+', '16+', '18+'),
    IN genre_arg ENUM('драма', 'комедия', 'триллер', 'детектив', 'блокбастер', 'мультфильм'),
    IN year_arg SMALLINT UNSIGNED,
    IN duration_arg SMALLINT UNSIGNED
)
exit_label: BEGIN
    DECLARE error VARCHAR(65);
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND role='admin';
    IF user_id_var IS NULL THEN
        SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    IF NOT EXISTS(SELECT * FROM movie WHERE id=movie_id_arg) THEN
        SET error = "NO MOVIE WITH THIS ID";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    START TRANSACTION;
    UPDATE movie SET 
        name = name_arg,
        movie_pg = movie_pg_arg,
        genre = genre_arg,
        year = year_arg,
        duration = duration_arg
    WHERE id=movie_id_arg;
    COMMIT;
END$$
DELIMITER ;