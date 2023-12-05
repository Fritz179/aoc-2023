use std::collections::VecDeque;

fn part1(input: &str) -> u64 {
    let mut segments = input.split("\n\n");

    let mut seeds: Vec<u64> = segments.next().unwrap().split_once(':').unwrap().1.trim().split(' ').map(|seed| seed.parse().unwrap()).collect();

    for segment in segments {
        let rules: Vec<(u64, u64, u64)> = segment.split_once(':').unwrap().1.trim().lines().map(|line| {
            let values: Vec<u64> = line.split(' ').map(|num| num.parse().unwrap()).collect();
            (values[0], values[1], values[2])
        }).collect();

        seeds = seeds.iter().map(|seed| {
            for (end, start, len) in rules.iter() {
                if *seed >= *start && *seed < *start + *len {
                    return *end + *seed - *start;
                }
            }

            *seed
        }).collect();
    }

    let mut record = seeds[0];
    seeds.iter().for_each(|seed| {
        if *seed < record {
            record = *seed
        }
    });

    record
}

fn part2(input: &str) -> u64 {
    let mut segments = input.split("\n\n");

    let mut data: VecDeque<u64> = segments.next().unwrap().split_once(':').unwrap().1.trim().split(' ').map(|seed| seed.parse().unwrap()).collect();
    let mut seeds: Vec<(u64, u64)> = Vec::with_capacity(data.len() / 2);

    while !data.is_empty() {
        let start = data.pop_front().unwrap();
        let range = data.pop_front().unwrap();

        seeds.push((start, start + range - 1));
    }

    for segment in segments {
        let rules: Vec<(u64, u64, u64)> = segment.split_once(':').unwrap().1.trim().lines().map(|line| {
            let values: Vec<u64> = line.split(' ').map(|num| num.parse().unwrap()).collect();
            (values[0], values[1], values[2])
        }).collect();
        // println!("seeds: {seeds:?}");
        // println!("rules: {rules:?}");


        let mut new_seeds = Vec::new();
        'outer: while let Some((seed_start, seed_end)) = seeds.pop() {
            for (dst, src, len) in rules.iter() {
                let start_in = seed_start >= *src && seed_start < *src + *len;
                let end_in = seed_end >= *src && seed_end < *src + *len;

                // println!("({seed_start},{seed_end}), start: {start_in}, end: {end_in}");
                if start_in && end_in {
                    new_seeds.push((*dst + seed_start - *src, *dst + seed_end - *src));
                    continue 'outer;
                } else if start_in {
                    new_seeds.push((*dst + seed_start - *src, *dst + *len - 1));
                    seeds.push((*src + *len, seed_end));
                    continue 'outer;
                } else if end_in {
                    seeds.push((seed_start, *src - 1));
                    new_seeds.push((*dst, *dst + seed_end - *src));
                    continue 'outer;
                }
            }

            new_seeds.push((seed_start, seed_end))

        }

        seeds = new_seeds;
    }

    let mut record = seeds[0];
    seeds.iter().for_each(|seed| {
        if *seed < record {
            record = *seed
        }
    });

    record.0
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
                "seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4"
            ),
            35
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4"
            ),
            46
        );
    }
}
