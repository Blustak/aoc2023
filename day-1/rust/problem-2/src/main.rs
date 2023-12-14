use std::collections::HashMap;
use std::env;
use std::fs;

struct IndexDigit(usize, char);

fn main() {
    let mut sum: u32 = 0;
    let translations = HashMap::from([
        ("one", '1'),
        ("two", '2'),
        ("three", '3'),
        ("four", '4'),
        ("five", '5'),
        ("six", '6'),
        ("seven", '7'),
        ("eight", '8'),
        ("nine", '9'),
    ]);
    let args: Vec<String> = env::args().collect();

    for arg in args.iter() {
        println!("{}", arg);
    }
    let lines: Vec<&str>;
    if let Ok(contents) = read_file(&args[1]) {
        lines = contents.split('\n').filter(|l| !(l.is_empty())).collect();

        for line in lines.iter() {
            let mut v: Vec<IndexDigit> = line
                .chars()
                .enumerate()
                .filter(|c| c.1.is_digit(10))
                .map(|c| IndexDigit(c.0, c.1))
                .collect();

            for ss in translations.keys() {
                if let Some(pos) = line.find(ss) {
                    v.push(IndexDigit(pos, translations[ss]));
                }
            }

            v.sort_by(|a, b| a.0.cmp(&b.0));

            sum += v
                .first()
                .unwrap()
                .1
                .to_digit(10)
                .expect("Couldn't convert first digit")
                * 10;
            sum += v
                .last()
                .unwrap()
                .1
                .to_digit(10)
                .expect("Couldn't convert last digit")
        }
    } else {
        panic!("Sorry, couldn't read file {}", &args[1]);
    }
    println!("The final sum i got was {}", sum);
}

fn read_file(file_path: &str) -> Result<String, std::io::Error> {
    fs::read_to_string(file_path)
}
