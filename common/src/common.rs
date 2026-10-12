use std::io;
use std::str::FromStr;

pub fn input<T: FromStr>() -> T where <T as FromStr>::Err: std::fmt::Debug {
    let mut line = String::new();
    io::stdin().read_line(&mut line).unwrap();
    line.trim().parse().expect("Failed to parse input")
}
