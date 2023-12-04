fn part1(input: &str) -> u32 {
    let mut total = 0;

    input.split('\n').for_each(|game| {
        let (_, deck) = game.split_once(':').unwrap();
        let (winners, have) = deck.split_once('|').unwrap();

        let winners: Vec<u32> = winners.split_whitespace().map(|el| {el.parse().unwrap()}).collect();
        let have: Vec<u32> = have.split_whitespace().map(|el| {el.parse().unwrap()}).collect();
        
        let mut count = 0;

        for i in winners {
            if have.contains(&i) {
                count += 1;
            }
        }

        if count > 0 {
            total += 2_u32.pow(count - 1);
        }
    });

    total
}

fn part2(input: &str) -> u32 {
    let mut total = 0;

    let mut lines: Vec<u32> = input.split('\n').map(|_| 1).collect();

    input.split('\n').enumerate().for_each(|(card, game)| {
        let (_, deck) = game.split_once(':').unwrap();
        let (winners, have) = deck.split_once('|').unwrap();

        let winners: Vec<u32> = winners.split_whitespace().map(|el| {el.parse().unwrap()}).collect();
        let have: Vec<u32> = have.split_whitespace().map(|el| {el.parse().unwrap()}).collect();
        
        let mut count = 0;
        for i in winners {
            if have.contains(&i) {
                count += 1;
            }
        }

        for i in (card + 1)..(card + 1 + count) {
            lines[i] += lines[card]
        }
    });

    for i in lines {
        total += i;
    }

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
        assert_eq!(
            part1(
                "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"
            ),
            13
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"
            ),
            30
        );
    }
}
