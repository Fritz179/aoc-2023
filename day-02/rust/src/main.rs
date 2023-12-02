fn part1(input: &str) -> u32 {
    const MAX_RED: u32 = 12;
    const MAX_GREEN: u32 = 13;
    const MAX_BLUE: u32 = 14;

    let mut total = 0;
    
    input.split('\n').for_each(|line| {
        let (game_num, games) = line.split_once(':').unwrap();

        let game_num = game_num.trim_start_matches("Game ").parse::<u32>().unwrap();

        let mut valid = true;
        games.split(';').for_each(|game| {
            let mut red = 0;
            let mut green = 0;
            let mut blue = 0;

            game.split(',').for_each(|item| {
                let item = item.trim();
                let count = item.split_whitespace().next().unwrap().parse::<u32>().unwrap();

                if item.ends_with("red") {
                    red += count;
                } else if item.ends_with("green") {
                    green += count;
                } else if item.ends_with("blue") {
                    blue += count;
                }
            });

            if red > MAX_RED || green > MAX_GREEN || blue > MAX_BLUE {
                valid = false;
            }
        
        });

        if valid {
            total += game_num;
        }
    });

    total
}

fn part2(input: &str) -> u32 {
    let mut total = 0;

    input.split('\n').for_each(|line| {
        let (_game_num, games) = line.split_once(':').unwrap();


        let mut red = 0;
        let mut green = 0;
        let mut blue = 0;

        games.split(';').for_each(|game| {
            game.split(',').for_each(|item| {
                let item = item.trim();
                let count = item.split_whitespace().next().unwrap().parse::<u32>().unwrap();
                
                if item.ends_with("red") {
                    if count > red {
                        red = count;
                    }
                } else if item.ends_with("green") {
                    if count > green {
                        green = count;
                    }
                } else if item.ends_with("blue") {
                    if count > blue {
                        blue = count;
                    }
                }
            });        
        });

        total += red * green * blue;
    });

    total
}

fn main() {
    let input = include_str!("../../input.txt");

    println!("Part 1: {}", part1(input));
    println!("Part 2: {}", part2(input));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_part1() {
        assert_eq!(part1("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"), 8);
    }

    #[test]
    fn test_part2() {
        assert_eq!(part2("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"), 2286);
    }
}