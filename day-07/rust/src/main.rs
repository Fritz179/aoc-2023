fn part1(input: &str) -> u32 {
    const VALUES: [char; 13] = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'];

    let mut lines: Vec<(u32, u32)> = input.lines().map(|line| {
        let (cards, value) = line.split_once(' ').unwrap();
        let value: u32 = value.parse().unwrap();

        let mut count = [0; 13];

        for card in cards.chars() {
            let index = VALUES.iter().position(|&c| c == card).unwrap();
            count[index] += 1;
        }

        // reverse sort
        count.sort_by(|a, b| b.cmp(a));

        let mut score = if count[0] > 3 {
            count[0] + 2
        } else if count[0] == 3 {
            if count[1] == 2 {
                5
            } else {
                4
            }
        } else if count[1] == 2 {
            3
        } else {
            count[0]
        };

        for card in cards.chars() {
            let index = VALUES.iter().position(|&c| c == card).unwrap();
            score = score * 13 + index as u32;
        }

        (score, value)
    }).collect();

    lines.sort_by(|a, b| a.0.cmp(&b.0));

    lines.iter().enumerate().fold(0, |acc, (pos, (_, value))| acc + (pos as u32 + 1) * value)
}

fn part2(input: &str) -> u32 {
    const VALUES: [char; 13] = ['J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'];

    let mut lines: Vec<(u32, u32)> = input.lines().map(|line| {
        let (cards, value) = line.split_once(' ').unwrap();
        let value: u32 = value.parse().unwrap();

        let mut count = [0; 13];

        for card in cards.chars() {
            let index = VALUES.iter().position(|&c| c == card).unwrap();
            count[index] += 1;
        }

        let jokers = count[0];
        count[0] = 0;

        // reverse sort
        count.sort_by(|a, b| b.cmp(a));

        count[0] += jokers;

        let mut score = if count[0] > 3 {
            count[0] + 2
        } else if count[0] == 3 {
            if count[1] == 2 {
                5
            } else {
                4
            }
        } else if count[1] == 2 {
            3
        } else {
            count[0]
        };

        for card in cards.chars() {
            let index = VALUES.iter().position(|&c| c == card).unwrap();
            score = score * 13 + index as u32;
        }

        (score, value)
    }).collect();

    lines.sort_by(|a, b| a.0.cmp(&b.0));

    lines.iter().enumerate().fold(0, |acc, (pos, (_, value))| acc + (pos as u32 + 1) * value)
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
        assert_eq!(
            part1(
                "32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483"
            ),
            6440
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483"
            ),
            5905
        );
    }
}
