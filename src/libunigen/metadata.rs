pub mod metadata {

    use std::fs::File;
    use std::io::Read;
    use std::string::String;

    pub const METADATA_MAGIC: [u8; 4] = [0xAF, 0x1B, 0xB1, 0xFA];

    pub struct MetadataObject {
        pub file: File,
        pub version: String,
    }

    pub enum MetadataError {
        FileInvalid,
        FileNotFound,
        FileUnreadable,
    }

    pub type MetadataResult = Result<MetadataObject, MetadataError>;

    impl MetadataError {
        pub fn to_string(&self) -> String {
            match *self {
                MetadataError::FileInvalid => String::from("Invalid File"),
                MetadataError::FileNotFound => String::from("File Not Found"),
                MetadataError::FileUnreadable => String::from("File Unreadable"),
            }
        }
    }

    impl MetadataObject {
        pub fn from_file(path: &str) -> MetadataResult {
            let mut file =
                File::open(path).unwrap_or_else(|_| panic!("Could not load metadata at {}", path));

            let mut buf = vec![0u8; 0x4];
            if let Err(_) = file.read_exact(&mut buf) {
                return Err(MetadataError::FileUnreadable);
            }

            if buf != METADATA_MAGIC {
                return Err(MetadataError::FileInvalid);
            }
            
            Ok(MetadataObject {
                file: file,
                version: "1.0.0".to_string(),
            })
        }

        pub fn is_valid() -> bool {
            true
        }
    }
}
