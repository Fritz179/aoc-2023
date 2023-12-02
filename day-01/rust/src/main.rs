fn part1(input: &str) -> u32 {
    let mut total = 0;
    
    input.split('\n').for_each(|line| {
        let mut last = None;
        let mut first = None;

        line.chars().for_each(|c| {
            if c.is_digit(10) {
                let digit = c.to_digit(10).unwrap();

                if last.is_none() {
                    first = Some(digit);
                }

                last = Some(digit);
            }
        });

        total += first.unwrap() * 10 + last.unwrap();
    });

    return total;
}

const NUMBERS: [&str; 9] = [
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
];

fn part2(input: &str) -> u32 {
    let mut total = 0;
    
    input.split('\n').for_each(|line| {
        let mut line = line.to_string();

        let mut last = None;
        let mut first = None;

        while line.len() > 0 {
            for (i, number) in NUMBERS.iter().enumerate() {
                if line.starts_with(number) {
                    if last.is_none() {
                        first = Some(i as u32 + 1);
                    }

                    last = Some(i as u32 + 1);
                    break;
                }

                let next = line.chars().next().unwrap();
                if next.is_digit(10) {
                    let digit = next.to_digit(10).unwrap();

                    if last.is_none() {
                        first = Some(digit);
                    }

                    last = Some(digit);
                    break;
                }
            }

            line.remove(0);
        }

        total += first.unwrap() * 10 + last.unwrap();
    });

    return total;
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
        assert_eq!(part1("1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet"), 142);
    }

    #[test]
    fn test_part2() {
        assert_eq!(part2("two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen"), 281);
    }
}