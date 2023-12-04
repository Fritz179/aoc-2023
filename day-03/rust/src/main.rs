fn part1(input: &str) -> u32 {
    let mut total = 0;

    let rows: Vec<&str> = input.lines().collect();

    let height = rows.len() as u32;
    let width = rows[0].len() as u32;

    for (y , row) in rows.iter().enumerate() {
        let y = y as u32;

        let line: Vec<char> = row.chars().collect();

        let mut start = None;
        let mut end = None;
        let mut count = 0;

        for x in 0..width {
            let digit = line[x as usize].to_digit(10);

            if let Some(digit) = digit {
                if start.is_none() {
                    start = Some(x);
                }

                end = Some(x);
                count = count * 10 + digit;
            } 
            
            if digit.is_none() || x == width - 1 {
                if let Some(start) = start {
                    if let Some(end) = end {
                        let x1 = if start > 0 { start - 1 } else { start };
                        let x2 = if end < width - 1 { end + 1 } else { end };

                        let y1 = if y > 0 { y - 1 } else { y };
                        let y2 = if y < height - 1 { y + 1 } else { y };

                        let mut valid = false;

                        'out: for y in y1..=y2 {
                            for x in x1..=x2 {
                                let char = rows[y as usize].chars().nth(x as usize).unwrap();
                                if char != '.' && !char.is_ascii_digit() {
                                    valid = true;
                                    break 'out;
                                }
                            }
                        }

                        if valid {
                            total += count;
                        }
                    }
                }

                start = None;
                end = None;
                count = 0;
            }
        }
    }

    total
}

fn part2(input: &str) -> u32 {
    let mut total = 0;

    let rows: Vec<&str> = input.lines().collect();

    let height = rows.len() as u32;
    let width = rows[0].len() as u32;

    let mut gears = Vec::new();

    for (y , row) in rows.iter().enumerate() {
        let y = y as u32;
        let line: Vec<char> = row.chars().collect();

        let mut start = None;
        let mut end = None;
        let mut count = 0;

        for x in 0..width {
            let digit = line[x as usize].to_digit(10);

            if let Some(digit) = digit {
                if start.is_none() {
                    start = Some(x);
                }

                end = Some(x);
                count = count * 10 + digit;
            } 
            
            if digit.is_none() || x == width - 1 {
                if let Some(start) = start {
                    if let Some(end) = end {
                        let x1 = if start > 0 { start - 1 } else { start };
                        let x2 = if end < width - 1 { end + 1 } else { end };

                        let y1 = if y > 0 { y - 1 } else { y };
                        let y2 = if y < height - 1 { y + 1 } else { y };

                        for y in y1..=y2 {
                            for x in x1..=x2 {
                                let char = rows[y as usize].chars().nth(x as usize).unwrap();
                                if char == '*' {
                                    if let Some((_, _, other)) = gears.iter().find(|(ox, oy, _)| { *ox == x && *oy == y}) {
                                        total += count * other;
                                    } else {
                                        gears.push((x, y, count));
                                    };

                                    // possible optimization: remove gears that are no longer valid
                                    // gears = gears.into_iter().filter(|(ox, oy, _)| { *ox != x || *oy != y}).collect();
                                }
                            }
                        }
                    }
                }

                start = None;
                end = None;
                count = 0;
            }
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
                "467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598.."
            ),
            4361
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            part2(
                "467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598.."
            ),
            467835
        );
    }
}
