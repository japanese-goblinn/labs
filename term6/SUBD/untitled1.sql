INSERT INTO `byTickets`.`country` (`name`) VALUES ('BELARUS');
INSERT INTO `byTickets`.`country` (`name`) VALUES ('USA');
INSERT INTO `byTickets`.`country` (`name`) VALUES ('RUSSIA');
INSERT INTO `byTickets`.`district` (`name`) VALUES ('FRUNZENSKY');
INSERT INTO `byTickets`.`living_place` (`name`, `type`) VALUES ('Minsk', 'город');
INSERT INTO `byTickets`.`street` (`name`, `type`) VALUES ('Frunzenskaya', 'улица');
INSERT INTO `byTickets`.`address` (`region`, `number`, `housing`, `living_place_id`, `street_id`, `district_id`, `country_id`) VALUES ('Минская', '44', '', '1', '1', '1', '1');
INSERT INTO `byTickets`.`bank` (`name`) VALUES ('PRIOR');
INSERT INTO `byTickets`.`bank_account` (`requisite`, `bank_id`) VALUES ('1234012349', '1');
INSERT INTO `byTickets`.`cinema` (`name`, `capacity`, `hall_amount`, `address_id`, `bank_account_id`) VALUES ('Belarus', '40', '5', '1', '1');
INSERT INTO `byTickets`.`movie` (`name`, `movie_pg`, `genre`, `year`, `duration`) VALUES ('Joker', '18+', 'блокбастер', '2019', '2');
INSERT INTO `byTickets`.`movie` (`name`, `movie_pg`, `genre`, `year`, `duration`) VALUES ('Bladerunner', '16+', 'блокбастер', '2017', '3');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '1', '1');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '2', '1');
INSERT INTO `byTickets`.`ticket` (`row`, `seat`, `hall`) VALUES ('1', '3', '1');

INSERT INTO `byTickets`.`store` (`ticket_id`, `price`, `movie_id`, `start`, `cinema_id`) VALUES ('1', '10', '1', '2021-01-01 13:00', '1');
INSERT INTO `byTickets`.`store` (`ticket_id`, `price`, `movie_id`, `start`, `cinema_id`) VALUES ('2', '20', '2', '2021-01-01 13:00', '1');
INSERT INTO `byTickets`.`store` (`ticket_id`, `price`, `movie_id`, `start`, `cinema_id`) VALUES ('3', '30', '1', '2021-01-01 13:00', '1');

CALL firstnameAndLastnameID('test', 'test');
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id, role) VALUES ('a', 'kek@gmail.com', '-8408485082223469184', '1', '1', 'admin');
CALL add_default_card("a", "PRIOR", 111111111, "2021-05-01");
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id, role) VALUES ('h', 'kek@gmail.com', '-8408485082223469184', '1', '1', 'headmaster');
CALL add_default_card("h", "PRIOR", 121111111, "2021-05-01");
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id, role) VALUES ('m', 'kek@gmail.com', '-8408485082223469184', '1', '1', 'manager');
CALL add_default_card("m", "PRIOR", 131111111, "2021-05-01");
INSERT INTO user (username, email, password_hash, firstname_id, lastname_id, role) VALUES ('u', 'kek@gmail.com', '-8408485082223469184', '1', '1', 'user');
CALL add_default_card("u", "PRIOR", 141111111, "2021-05-01");
