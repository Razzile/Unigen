use clap::{load_yaml, App};
use std::fs::File;

fn main() {
    let config = load_yaml!("../../config.yml");
    let matches = App::from_yaml(config).get_matches();

    let binary = matches.value_of("binary").unwrap();
    let metadata = matches.value_of("metadata").unwrap();

    println!("binary: {} metadata: {}", binary, metadata);

    let binary_file =
        File::open(binary).unwrap_or_else(|_| panic!("Could not load binary at {}", binary));
    let metadata_file =
        File::open(metadata).unwrap_or_else(|_| panic!("Could not load metadata at {}", metadata));
}
