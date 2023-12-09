fn part1(input: &str) -> u32 {
    let mut total = 0;

    input.lines().for_each(|line| {
        let mut derivate = Vec::new();

        let line: Vec<i32> = line.split_whitespace().map(|x| x.parse().unwrap()).collect();
        derivate.push(line);
    
        loop {
            let last = derivate.last().unwrap();
            if last.iter().all(|x| *x == 0) {
                break;
            }

            let mut new = Vec::new();
            for i in 1..last.len() {
                new.push(last[i] - last[i - 1])
            }

            derivate.push(new);
        }
        
        derivate.last_mut().unwrap().push(0);

        for i in (0..derivate.len() - 2).rev() {
            let val = derivate[i].last().unwrap() + derivate[i + 1].last().unwrap();
            derivate[i].push(val);
        }

        total += derivate[0].last().unwrap();
    });

    total as u32
}

fn part2(input: &str) -> u32 {
    let mut total = 0;

    input.lines().for_each(|line| {
        let mut derivate = Vec::new();

        let line: Vec<i32> = line.split_whitespace().rev().map(|x| x.parse().unwrap()).collect();
        derivate.push(line);
    
        loop {
            let last = derivate.last().unwrap();
            if last.iter().all(|x| *x == 0) {
                break;
            }

            let mut new = Vec::new();
            for i in 1..last.len() {
                new.push(last[i] - last[i - 1])
            }

            derivate.push(new);
        }
        
        derivate.last_mut().unwrap().push(0);

        for i in (0..derivate.len() - 2).rev() {
            let val = derivate[i].last().unwrap() + derivate[i + 1].last().unwrap();
            derivate[i].push(val);
        }

        total += derivate[0].last().unwrap();
    });

    total as u32
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
                "0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45"
            ),
            114
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45"
            ),
            2
        );
    }
}
