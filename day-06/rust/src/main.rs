fn part1(input: &str) -> u32 {
    let (times, distances) = input.split_once('\n').unwrap();
    let times: Vec<u32> = times.split_once(':').unwrap().1.split_whitespace().map(|num| num.parse().unwrap()).collect();
    let distances: Vec<u32> = distances.split_once(':').unwrap().1.split_whitespace().map(|num| num.parse().unwrap()).collect();

    let mut total = 1;

    times.iter().zip(distances.iter()).for_each(|(time, distance)| {
        let mut count = 0;
        for i in 0..*time {
            if i * (*time - i) > *distance {
                count += 1;
            }
        }

        total *= count;
    });
    
    total
}

fn part2(input: &str) -> u32 {
    let (times, distances) = input.split_once('\n').unwrap();
    let time: f32 = times.split_once(':').unwrap().1.replace(' ', "").parse().unwrap();
    let distance: f32 = distances.split_once(':').unwrap().1.replace(' ', "").parse().unwrap();

    // x * (d - x) == t
    // x * t - x ** 2 -t == 0
    // a = -1, b = t, c = -t
    
    let a = -1.0;
    let b = time;
    let c = -distance;

    let d = f32::sqrt(b.powi(2) - 4.0 * a * c);
    let x1 = ((-b + d) / (2.0 * a)).ceil() as u32;
    let x2 = ((-b - d) / (2.0 * a)).ceil() as u32;

    x2 - x1
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
                "Time:      7  15   30
Distance:  9  40  200"
            ),
            288
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "Time:      7  15   30
Distance:  9  40  200"
            ),
            71503
        );
    }
}
