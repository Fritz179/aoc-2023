fn part1(input: &str) -> u32 {
    let mut map = std::collections::HashMap::new();

    let (sequence, table) = input.split_once('\n').unwrap();
    let sequence: Vec<char> = sequence.chars().collect();

    table.trim().lines().for_each(|line| {
        let (key, value) = line.split_once('=').unwrap();
        let key = key.trim();
        let value = value.trim().trim_matches(|c| c == '(' || c == ')').split_once(", ").unwrap();

        map.insert(key, value);
    });

    let mut curr = "AAA";

    for (i, dir) in sequence.iter().cycle().enumerate() {
        if curr == "ZZZ" {
            return i as u32;
        }

        let (left, right) = map.get(curr).unwrap();
        if *dir == 'L' {
            curr = left;
        } else {
            curr = right;
        }
    }

    unreachable!("Should never reach here");
}

fn part2(input: &str) -> u64 {
    let mut map = std::collections::HashMap::new();

    let (sequence, table) = input.split_once('\n').unwrap();
    let sequence: Vec<char> = sequence.chars().collect();

    let mut curr = Vec::new();
    table.trim().lines().for_each(|line| {
        let (key, value) = line.split_once('=').unwrap();
        let key = key.trim();
        let (left, right) = value.trim().trim_matches(|c| c == '(' || c == ')').split_once(", ").unwrap();

        let end = key.chars().nth(2).unwrap() == 'Z';

        map.insert(key, (left, right, end));

        if key.chars().nth(2).unwrap() == 'A' {
            curr.push(key);
        }
    });

    let cycels: Vec<u32> = curr.iter().map(|start| {
        let mut curr = start;

        for (i, dir) in sequence.iter().cycle().enumerate() {    
            let (left, right, end) = map.get(curr).unwrap();

            if *end {
                return i as u32;
            }

            if *dir == 'L' {
                curr = left;
            } else {
                curr = right;
            }
        }

        unreachable!("Should never reach here");
    }).collect();

    const SIZE: usize = 500;
    let mut record = vec![0; SIZE];

    cycels.iter().for_each(|cycle| {
        let mut cycle = *cycle;
        let mut curr = vec![0; SIZE];

        for i in 2..SIZE {
            if cycle % i as u32 == 0 {
                curr[i] += 1;
                cycle /= i as u32;
            }

            if curr[i] > record[i] {
                record[i] = curr[i];
            }
        }
    });

    let mut total = 1;

    for i in 2..SIZE {
        if record[i] > 0 {
            total *= i as u64 * record[i];
        }
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
                "LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ)"
            ),
            6
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "LR

11A = (11B, XXX)
11B = (XXX, 11Z)
11Z = (11B, XXX)
22A = (22B, XXX)
22B = (22C, 22C)
22C = (22Z, 22Z)
22Z = (22B, 22B)
XXX = (XXX, XXX)"
            ),
            6
        );
    }
}
