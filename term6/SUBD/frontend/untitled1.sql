
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
    UPDATE user SET default_card_id=card_id;
    COMMIT;
END$$

DELIMITER ;


-- Возможность покупки билетов (все пользователи);

INSERT INTO `byTickets`.`country` (`name`) VALUES ('BELARUS');
INSERT INTO `byTickets`.`country` (`name`) VALUES ('USA');
INSERT INTO `byTickets`.`country` (`name`) VALUES ('RUSSIA');
INSERT INTO `byTickets`.`district` (`name`) VALUES ('FRUNZENSKY');
INSERT INTO `byTickets`.`living_place` (`name`, `type`) VALUES ('Minsk', 'город');
INSERT INTO `byTickets`.`street` (`name`, `type`) VALUES ('Frunzenskaya', 'улица');
INSERT INTO `byTickets`.`address` (`region`, `number`, `housing`, `living_place_id`, `street_id`, `district_id`, `country_id`) VALUES ('Минская', '44', '', '1', '1', '1', '1');
INSERT INTO `byTickets`.`bank_account` (`requisite`, `bank_id`) VALUES ('1234012349', '5');
INSERT INTO `byTickets`.`cinema` (`name`, `capacity`, `hall_amount`, `address_id`, `bank_account_id`) VALUES ('Belarus', '40', '5', '1', '1');
INSERT INTO `byTickets`.`movie` (`name`, `movie_pg`, `genre`, `year`, `duration`) VALUES ('Joker', '18+', 'блокбастер', '2019', '2');
INSERT INTO `byTickets`.`movie` (`name`, `movie_pg`, `genre`, `year`, `duration`) VALUES ('Bladerunner', '16+', 'блокбастер', '2017', '3');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '1', '1');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '2', '1');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '3', '1');

USE `byTickets`;
DROP procedure IF EXISTS `buy_ticket`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `buy_ticket`(
	IN username_arg VARCHAR(32),
	IN cinema_name_arg VARCHAR(32),
    IN movie_name_arg VARCHAR(128),
    IN when_arg TIMESTAMP
)
exit_label: BEGIN
	DECLARE error VARCHAR(65);
    DECLARE var_card_id MEDIUMINT UNSIGNED;
	DECLARE var_cinema_id TINYINT UNSIGNED;
    DECLARE var_movie_id INT UNSIGNED;
    DECLARE var_store_id INT UNSIGNED;
    DECLARE ticket_id_var INT UNSIGNED;
    DECLARE ticket_price_var INT UNSIGNED;
    DECLARE transaction_id_var INT UNSIGNED;
    DECLARE user_id_var MEDIUMINT UNSIGNED;
    DECLARE sale_id_var INT UNSIGNED;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    
    IF when_arg <= CURRENT_TIMESTAMP() THEN
		SET error = "ORDER DATE IS WRONG";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg;
    IF user_id_var IS NULL THEN
		SET error = "NO USER WITH THIS USERNAME";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT default_card_id INTO var_card_id FROM user WHERE username=username_arg;
    IF var_card_id IS NULL THEN
		SET error = "NO CARD ASSIGNED TO ACCOUNT";
        SELECT error;
        LEAVE exit_label;
    END IF;
	
    SELECT id INTO var_cinema_id FROM cinema WHERE name=cinema_name_arg;
    IF var_cinema_id IS NULL THEN
		SET error = "CINEMA WITH THIS NAME NOT FOUNDED";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT id INTO var_movie_id FROM movie WHERE name=movie_name_arg;
    IF var_movie_id IS NULL THEN
		SET error = "MOVIE WITH THIS NAME NOT FOUNDED";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    SELECT 
		id, ticket_id, price
	INTO 
		var_store_id, ticket_id_var, ticket_price_var  
	FROM store WHERE 
		movie_id=var_movie_id AND cinema_id=var_cinema_id AND start=when_arg LIMIT 1;
	IF var_store_id IS NULL THEN
		SET error = "NO TICKETS ON THIS DATE";
        SELECT error;
        LEAVE exit_label;
    END IF;
    
    START TRANSACTION;
    INSERT INTO transaction SET order_sum=ticket_price_var, card_id=var_card_id;
    SELECT id INTO transaction_id_var FROM transaction WHERE order_sum=ticket_price_var AND card_id=var_card_id LIMIT 1;
    INSERT INTO sale SET sale_date=NOW(), transaction_id=transaction_id_var, user_id=user_id_var;
    SELECT id INTO sale_id_var FROM sale WHERE transaction_id=transaction_id_var AND user_id=user_id_var LIMIT 1;
    UPDATE store SET sale_id=sale_id_var WHERE id=var_store_id;
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
    
    SELECT COUNT(id) INTO amount FROM store;
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
		t.id, t.order_sum money_transfered,
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
    
    SELECT id INTO user_id_var FROM user WHERE username=username_arg AND role='headmaster';
    IF user_id_var IS NULL THEN
		SET error = "NO USER WITH THIS USERNAME OR NO RIGHTS TO ACCESS";
        SELECT error;
        LEAVE exit_label;
    END IF;
     
    SELECT 
		s.id, s.sale_date date,
		t.order_sum money_transfered, 
        u.username to_user,
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
DROP procedure IF EXISTS `see_not_available_tickets`;

DELIMITER $$
USE `byTickets`$$
CREATE DEFINER=`root`@`%` PROCEDURE `see_not_available_tickets`()
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
		s.sale_id IS NOT NULL; 
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
		s.start=date;
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
		m.name=movie_arg;
END$$

DELIMITER ;


-- Поиск билетов по определённому кинотеатру (все пользователи);



Фильтрация билетов по кинотеатру (все пользователи);

Фильтрация билетов по времени сеанса (все пользователи);

Фильтрация билетов по фильму (все пользователи);

Возможность отмены заказов пользователей (администратор);

Возможность редактирования различной информации о фильмах, ценах на билеты и т.п (администратор);

























