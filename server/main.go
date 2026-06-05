package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"net"
)

type InputPacket struct {
	Buttons uint32
	CircleX int16
	CircleY int16
}

const (
	KEY_A      uint32 = 1 << 0
	KEY_B      uint32 = 1 << 1
	KEY_SELECT uint32 = 1 << 2
	KEY_START  uint32 = 1 << 3
	KEY_DRIGHT uint32 = 1 << 4
	KEY_DLEFT  uint32 = 1 << 5
	KEY_DUP    uint32 = 1 << 6
	KEY_DDOWN  uint32 = 1 << 7
	KEY_R      uint32 = 1 << 8
	KEY_L      uint32 = 1 << 9
	KEY_X      uint32 = 1 << 10
	KEY_Y      uint32 = 1 << 11
)

func parseButtons(mask uint32) []string {
	var pressed []string
	if mask&KEY_A != 0 { pressed = append(pressed, "A") }
	if mask&KEY_B != 0 { pressed = append(pressed, "B") }
	if mask&KEY_X != 0 { pressed = append(pressed, "X") }
	if mask&KEY_Y != 0 { pressed = append(pressed, "Y") }
	if mask&KEY_L != 0 { pressed = append(pressed, "L") }
	if mask&KEY_R != 0 { pressed = append(pressed, "R") }
	if mask&KEY_DUP != 0 { pressed = append(pressed, "D-Up") }
	if mask&KEY_DDOWN != 0 { pressed = append(pressed, "D-Down") }
	if mask&KEY_DLEFT != 0 { pressed = append(pressed, "D-Left") }
	if mask&KEY_DRIGHT != 0 { pressed = append(pressed, "D-Right") }
	if mask&KEY_SELECT != 0 { pressed = append(pressed, "Select") }
	if mask&KEY_START != 0 { pressed = append(pressed, "Start") }
	return pressed
}

func getActiveLocalIP() (net.IP, error) {
	conn, err := net.Dial("udp", "8.8.8.8:80")
	if err != nil {
		return nil, err
	}
	defer conn.Close()

	localAddr := conn.LocalAddr().(*net.UDPAddr)
	return localAddr.IP, nil
}

func main() {
	localIP, err := getActiveLocalIP()
	if err != nil {
		log.Fatalf("Error: %v", err)
	}

	addr := &net.UDPAddr{
		IP:   net.IPv4zero,
		Port: 0,
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		log.Fatalf("Error: %v", err)
	}
	defer conn.Close()

	assignedAddr := conn.LocalAddr().(*net.UDPAddr)

	fmt.Printf("IP to type into 3DS: %s\n", localIP)
	fmt.Printf("Port to type into 3DS: %d\n", assignedAddr.Port)

	buffer := make([]byte, 8)
	fmt.Println("Waiting for UDP packets... Press Ctrl+C to exit.")

	for {
		n, _, err := conn.ReadFromUDP(buffer)
		if err != nil {
			log.Printf("Error: %v", err)
			continue
		}

		if n != 8 {
			continue
		}

		var packet InputPacket
		reader := bytes.NewReader(buffer[:n])
		err = binary.Read(reader, binary.LittleEndian, &packet)
		if err != nil {
			log.Printf("Decoding error: %v", err)
			continue
		}

		pressedButtons := parseButtons(packet.Buttons)

		fmt.Printf("\rStick: X=%4d Y=%4d | Buttons: %v             ", 
			packet.CircleX, packet.CircleY, pressedButtons)
	}
}
